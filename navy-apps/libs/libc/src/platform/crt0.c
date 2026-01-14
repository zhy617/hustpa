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
  write(1, &ret, sizeof(ret)); // debug
  exit(ret);
  assert(0);
}
