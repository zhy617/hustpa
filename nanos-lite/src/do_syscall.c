#include "common.h"
#include "syscall.h"

int sys_yield() {
  _yield();
  return 0;
}

int sys_exit(int status) {
  printf("Program exited with status %d\n", status);
  _halt(status);
  return 0; 
}

size_t sys_write(int fd, const void *buf, size_t len) {
  // For simplicity, we only handle fd = 1 (stdout) or fd = 2 (stderr)
  printf("sys_write called with fd=%d, len=%d\n", fd, len);
  printf("buf addr = %x\n", buf);
  // addr = 0x830069c7
  // addr = 0x83005228
  // w $pc==0x80100510
  for (size_t i = 0; i < 100; i++) {
    printf("buf[%d] = %c (0x%x)\n", i, ((char *)buf)[i], ((char *)buf)[i]);
  }
  if (fd == 1 || fd == 2) {
    for (size_t i = 0; i < len; i++) {
      _putc(((char *)buf)[i]);
    }
    return len; // Return number of bytes written
  } else {
    return -1; // Unsupported fd
  }
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_yield:
      c->GPRx = sys_yield();
      return c;
      break;
    case SYS_exit:
      c->GPRx = sys_exit(a[1]);
      break;
    case SYS_write:
      c->GPRx = sys_write(a[1], (const void *)a[2], a[3]);
      return c;
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
