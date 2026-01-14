#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;

void _start(int argc, char *argv[], char *envp[]) {
  char *env[] = {NULL};
  environ = env;
  int ret = main(argc, argv, envp);
  // write(1, &ret, sizeof(ret)); // debug
  // write(1, "Exit code: ", 11); // debug
  // write(1, (char *)&ret, sizeof(ret)); // debug
  // write(1, "\n", 1); // debug
  // printf("Program exited with return value %d\n", ret);
  _exit(ret);
  assert(0);
}
