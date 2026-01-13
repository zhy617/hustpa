#include "rtl/rtl.h"
#include "cpu/decode.h"

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  decinfo.isa.sepc = epc;
  decinfo.isa.scause = NO;
  rtl_j(decinfo.isa.stvec);
}

bool isa_query_intr(void) {
  return false;
}
