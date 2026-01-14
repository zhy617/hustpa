#include "common.h"
#include "do_syscall.h"

static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
    case _EVENT_YIELD:
      printf("Handling yield event\n");
      // _yield();
      // TODO
      c->GPRx = 0;
      return c;
      break;
    case _EVENT_SYSCALL:
      // printf("Handling syscall event\n");
      // printf("GPR3 = %d\n", c->GPR3);
      return do_syscall(c);

    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
