#include "rtl/rtl.h"
#include "cpu/decode.h"

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  cpu.sepc = epc;
  cpu.scause = NO;
  printf("cpu.sepc: 0x%x\n", cpu.sepc);
  printf("cpu.scause: 0x%x\n", cpu.scause);
  decinfo.jmp_pc = cpu.stvec;
  printf("stvec: 0x%x\n", cpu.stvec);
  rtl_j(cpu.stvec);
}

bool isa_query_intr(void) {
  return false;
}
