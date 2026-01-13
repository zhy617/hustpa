#include "rtl/rtl.h"
#include "cpu/decode.h"

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  cpu.sepc = epc;
  cpu.scause = NO;
  decinfo.jmp_pc = cpu.stvec;
  printf("stvec: 0x%x\n", cpu.stvec);
  rtl_j(decinfo.isa.stvec);
}

bool isa_query_intr(void) {
  return false;
}
