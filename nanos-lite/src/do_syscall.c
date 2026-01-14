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
    case SYS_write:
      {
        int fd = c->GPR2;
        const void *buf = (const void *)c->GPR3;
        size_t len = c->GPR4;
        // For simplicity, we only handle fd = 1 (stdout) or fd = 2 (stderr)
        if (fd == 1 || fd == 2) {
          for (size_t i = 0; i < len; i++) {
            _putc(((char *)buf)[i]);
          }
          c->GPRx = len; // Return number of bytes written
        } else {
          c->GPRx = -1; // Unsupported fd
        }
      }
      return c;
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
