#include "nemu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int init_monitor(int, char *[]);
void ui_mainloop(int);

uint32_t expr(char *e, bool *success); 

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
  printf("%d\n", is_batch_mode);
  if (is_batch_mode) {  
    FILE *fp = fopen("/home/mystle/ics2019/nemu/tools/gen-expr/input", "r");
    if (fp == NULL) {
      printf("Cannot open input file\n");
      return 1;
    }

    char line[65536];
    int line_num = 0;
    int success_count = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
      line_num++;
      char *p = strtok(line, " ");
      if (p == NULL) continue;

      uint32_t expected_result = strtoul(p, NULL, 10);
      
      p = strtok(NULL, "\n");
      if (p == NULL) continue;

      bool success = true;
      uint32_t actual_result = expr(p, &success);

      if (success && actual_result == expected_result) {
        success_count++;
      } else {
        printf("Test failed at line %d: %s\n", line_num, p);
        printf("Expected: %u, Got: %u\n", expected_result, actual_result);
      }
    }
    fclose(fp);
    printf("Batch test summary: %d / %d tests passed.\n", success_count, line_num);
    return 0;
  }
  else {
    ui_mainloop(is_batch_mode);
  }
}
