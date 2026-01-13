#include <am.h>
#include <riscv32.h>

#define IRQ_UECALL 8

static _Context* (*user_handler)(_Event, _Context*) = NULL;

_Context* __am_irq_handle(_Context *c) {
  _Context *next = c;
  // for (int i = 0; i < 32; i++) {
  //   printf("gpr[%d] = %x\n", i, c->gpr[i]);
  // }
  // printf("irq: cause = %x\n", c->cause);
  // printf("irq: epc = %x\n", c->epc);
  // printf("irq: status = %x\n", c->status);
  uint32_t* ptr = c;
  for (int i = 0; i < 40; i++) {
    printf("context[%d] = %x, addr = %x\n", i, ptr[i], &ptr[i]);
  }
  if (user_handler) {
    _Event ev = {0};
    switch (c->cause) {
      case IRQ_UECALL: ev.event = _EVENT_YIELD; printf("Yield event\n"); break;
      default: ev.event = _EVENT_ERROR; break;
    }
    
    next = user_handler(ev, c);
    if (next == NULL) {
      next = c;
    }
  }

  return next;
}

extern void __am_asm_trap(void);

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize exception entry
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  printf("_cte_init called\n");
  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  return NULL;
}

void _yield() {
  asm volatile("li a7, -1; ecall");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
