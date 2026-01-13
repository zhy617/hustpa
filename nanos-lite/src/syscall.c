#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield:
      _yield();
      c->GPRx = 0;
      return c;
      break;
    case SYS_exit:
      // Handle exit syscall
      printf("Program exited with status %d\n", c->GPR2);
      // Perform any necessary cleanup here
      _halt(c->GPR2);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
