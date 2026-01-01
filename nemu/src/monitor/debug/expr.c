#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include "isa/reg.h"

enum {
  TK_NOTYPE = 256, 
  TK_EQ,
  /* TODO: Add more token types */
  TK_NEQ,
  TK_AND,
  TK_OR,
  TK_NUM,
  TK_HEX,
  TK_REG,
  TK_DEREF,
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {"0[xX][0-9a-fA-F]+", TK_HEX}, // hexadecimal numbers
  {"\\$[a-zA-Z][a-zA-Z0-9]*", TK_REG}, // registers
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},          // minus
  {"\\*", '*'},         // multiply
  {"/", '/'},           // divide
  {"\\(", '('},         // left parenthesis
  {"\\)", ')'},         // right parenthesis
  {"==", TK_EQ},         // equal
  {"!=", TK_NEQ},        // not equal
  {"&&", TK_AND},        // and
  {"\\|\\|", TK_OR},     // or
  {"[0-9]+", TK_NUM},    // numbers
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        if (nr_token >= 32) {
          printf("too many tokens\n");
          return false;
        }

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case TK_NUM:
          case TK_HEX:
          case TK_REG:
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case TK_EQ:
          case TK_NEQ:
          case TK_AND:
          case TK_OR:
          default:
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
            break;
          // default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static bool check_parentheses(int p, int q, bool *success) {
  if (tokens[p].type != '(' || tokens[q].type != ')') {
    return false;
  }
  int cnt = 0;
  for (int i = p + 1; i < q; i++) {
    if (tokens[i].type == '(') {
      cnt++;
    } else if (tokens[i].type == ')') {
      if (cnt == 0) {
        return false;
      }
      cnt--;
    }
  }
  if (cnt != 0) {
    *success = false;
    return false;
  }
  return true;
}

uint32_t eval(int p, int q, bool *success) {
  // TODO();
  if (p > q) {
    *success = false;
    return 0;
  } else if (p == q) {
    // single token
    uint32_t val = 0;
    switch (tokens[p].type) {
      case TK_NUM:
        sscanf(tokens[p].str, "%d", &val);
        return val;
      case TK_HEX:
        sscanf(tokens[p].str, "%x", &val);
        return val;
      case TK_REG: {
        bool reg_success = true;
        val = isa_reg_str2val(tokens[p].str + 1, &reg_success);
        if (!reg_success) {
          *success = false;
          return 0;
        }
        return val;
      }
      default:
        *success = false;
        return 0;
    }
  } else if (check_parentheses(p, q, success) == true) {
    // expression surrounded by a matched pair of parentheses
    return eval(p + 1, q - 1, success);
  } else {
    // TODO();
    // find main operator
    int main_op = -1;
    int min_precedence = 100;
    int cnt = 0;
    for (int i = p; i <= q; i++) {
      if (tokens[i].type == '(') {
        cnt++;
      } else if (tokens[i].type == ')') {
        cnt--;
      } else if (cnt == 0) {
        int precedence = 100;
        switch (tokens[i].type) {
          case TK_OR:
            precedence = 1;
            break;
          case TK_AND:
            precedence = 2;
            break;
          case TK_EQ:
          case TK_NEQ:
            precedence = 3;
            break;
          case '+':
          case '-':
            precedence = 4;
            break;
          case '*':
          case '/':
            precedence = 5;
            break;
          case TK_DEREF:
            precedence = 6;
            break;
          default:
            break;
        }
        if (precedence < min_precedence || (precedence == min_precedence && tokens[i].type != TK_DEREF)) {
          min_precedence = precedence;
          main_op = i;
        }
      }
    }
    if (main_op == -1) {
      *success = false;
      return 0;
    }

    if (tokens[main_op].type == TK_DEREF) {
      uint32_t addr = eval(main_op + 1, q, success);
      if (!(*success)) {
        return 0;
      }
      return vaddr_read(addr, 4);
    }

    uint32_t val1 = eval(p, main_op - 1, success);
    if (!(*success)) {
      return 0;
    }
    uint32_t val2 = eval(main_op + 1, q, success);
    if (!(*success)) {
      return 0;
    }
    switch (tokens[main_op].type) {
      case TK_OR:
        return val1 || val2;
      case TK_AND:
        return val1 && val2;
      case TK_EQ:
        return val1 == val2;
      case TK_NEQ:
        return val1 != val2;
      case '+':
        return val1 + val2;
      case '-':
        return val1 - val2;
      case '*':
        return val1 * val2;
      case '/':
        if (val2 == 0) {
          *success = false;
          return 0;
        }
        return val1 / val2;
      default:
        *success = false;
    }
  }
  return 0;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();

  for (int i = 0; i < nr_token; i ++) {
    if (tokens[i].type == '*') {
      if (i == 0 || (tokens[i - 1].type != TK_NUM && tokens[i - 1].type != TK_HEX && 
                      tokens[i - 1].type != TK_REG && tokens[i - 1].type != ')')) {
        tokens[i].type = TK_DEREF;
      }
    }
    printf("token[%d]: type=%d, str=%s\n", i, tokens[i].type, tokens[i].str);
  }

  uint32_t result = eval(0, nr_token - 1, success);
  return result;
}
