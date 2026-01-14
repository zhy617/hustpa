#include <unistd.h>
#include <stdio.h>

int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  while (i < 10) {
    j ++;
    write(1, "looping...\n", 11);
    if (j == 1) {
      
      printf("Hello World from Navy-apps for the %dth time!\n", i);
      
      // write(1, "Hello World from Navy-apps!\n", 29);
      i ++;

      j = 0;
    }
  }
  return 0;
}
