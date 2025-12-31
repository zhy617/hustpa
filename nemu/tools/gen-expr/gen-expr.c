#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


// --- Begin: Expression Evaluator ---
static const char *eval_p; // Pointer to the current position in the expression string
static int eval_error;    // Flag to indicate an error, e.g., division by zero

// Forward declarations for recursive parsing
static long eval_expr();

// Skips whitespace
static void skip_spaces() {
  while (*eval_p == ' ') {
    eval_p++;
  }
}

// Parses numbers and parentheses
// factor := number | '(' expr ')'
static long eval_factor() {
  skip_spaces();
  if (*eval_p == '(') {
    eval_p++; // Consume '('
    long val = eval_expr();
    skip_spaces();
    if (*eval_p == ')') {
      eval_p++; // Consume ')'
      return val;
    }
    // Mismatched parentheses or other syntax error
    eval_error = 1;
    return 0;
  } else if (isdigit(*eval_p)) {
    return strtol(eval_p, (char **)&eval_p, 10);
  }
  
  // Syntax error
  eval_error = 1;
  return 0;
}

// Parses multiplication and division
// term := factor { ('*' | '/') factor }
static long eval_term() {
  long val = eval_factor();
  if (eval_error) return 0;

  while (1) {
    skip_spaces();
    if (*eval_p == '*') {
      eval_p++;
      long rhs = eval_factor();
      if (eval_error) return 0;
      val *= rhs;
    } else if (*eval_p == '/') {
      eval_p++;
      long rhs = eval_factor();
      if (eval_error) return 0;
      if (rhs == 0) {
        // Division by zero detected!
        eval_error = 1;
        return 0;
      }
      val /= rhs;
    } else {
      break;
    }
  }
  return val;
}

// Parses addition and subtraction
// expr := term { ('+' | '-') term }
static long eval_expr() {
  long val = eval_term();
  if (eval_error) return 0;

  while (1) {
    skip_spaces();
    if (*eval_p == '+') {
      eval_p++;
      val += eval_term();
      if (eval_error) return 0;
    } else if (*eval_p == '-') {
      eval_p++;
      val -= eval_term();
      if (eval_error) return 0;
    } else {
      break;
    }
  }
  return val;
}

// Main function to evaluate an expression string
// Returns true on success, false on failure (e.g., division by zero)
static int evaluate(const char *expr) {
  eval_p = expr;
  eval_error = 0;
  eval_expr();
  // The expression is valid if we parsed without error and consumed the whole string
  skip_spaces();
  return !eval_error && *eval_p == '\0';
}
// --- End: Expression Evaluator ---

// this should be enough
static char buf[65536];

static void append(const char *s) {
  strcat(buf, s);
}

// static void gen_space() {
//   int r = rand() % 4;
//   for (int i = 0; i < r; i ++) {
//     append(" ");
//   }
// }

static void gen_num() {
  char num_buf[32];
  int r = rand() % 100;
  sprintf(num_buf, "%d", r);
  append(num_buf);
}

static void generate(int depth) {
  // 递归深度过深或随机到达基本情况，则生成一个数字
  if (depth <= 0 || (rand() % 10 < 3)) {
    gen_num();
    return;
  }

  // 随机选择生成 (expr) 或 expr op expr
  int choice = rand() % 3;
  if (choice == 0) {
    // 生成 (expr)
    append("(");
    // gen_space();
    generate(depth - 1);
    // gen_space();
    append(")");
  } else {
    // 生成 expr op expr
    generate(depth - 1);
    // gen_space();

    // 随机选择一个操作符
    char op;
    switch (rand() % 5) {
      case 0: op = '+'; break;
      case 1: op = '-'; break;
      case 2: op = '*'; break;
      case 3: op = '/'; break;
      case 4: op = '&'; break;
      default: op = '|'; break;
    }
    char op_str[2] = {op, '\0'};
    append(op_str);

    // gen_space();
    generate(depth - 1);
  }
}

static inline void gen_rand_expr() {
  buf[0] = '\0';
  //TODO();
  generate(rand() % 2 + 2);
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    while(1){
      gen_rand_expr();
      // printf("%s\n", buf);
      if (evaluate(buf)) break;
    }

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
