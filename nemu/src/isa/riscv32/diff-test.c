#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  for (int i = 0; i < 32; i++) {
    if ((ref_r->gpr[i]._32) != cpu.gpr[i]._32) {
      printf("Differential testing failed at PC = 0x%x\n", pc);
      printf("Register '%s' mismatch: ref = 0x%x, dut = 0x%x\n",
             reg_name(i, 32), ref_r->gpr[i]._32, cpu.gpr[i]._32);
      return false;
    }
  }

  if (ref_r->pc != cpu.pc) {
    printf("Differential testing failed at PC = 0x%x\n", pc);
    printf("PC mismatch: ref = 0x%x, dut = 0x%x\n", ref_r->pc, cpu.pc);
    return false;
  }

  return true;
}

void isa_difftest_attach(void) {
}
