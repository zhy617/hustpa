#include "cpu/exec.h"

make_EHelper(jalr) {
  difftest_skip_dut(1, 2);

  // 1. Calculate target address: t0 = rs1 + imm
  rtl_add(&t0, &id_src->val, &id_src2->val);
  // Align to 2 bytes by clearing the LSB: t0 = t0 & ~1
  rtl_andi(&t0, &t0, ~1);

  // 2. Save link address: rd = pc + 4
  // decinfo.seq_pc is pc + 4
  rtl_sr(id_dest->reg, &decinfo.seq_pc, 4);

  // 3. Perform the jump
  rtl_j(t0);

  print_asm_template2(jalr);
}

make_EHelper(jal) {
  // 1. Save link address: rd = pc + 4
  // decinfo.seq_pc is pc + 4
  rtl_sr(id_dest->reg, &decinfo.seq_pc, 4);

  // 2. pc = pc + imm - 4
  rtl_add(&t0, &decinfo.seq_pc, &id_src->val);
  rtl_subi(&t0, &t0, 4);
  // printf("t0 = 0x%08x\n", t0);
  rtl_j(t0);

  print_asm_template2(jal);
}

make_EHelper(beq) {
  if (id_src->val == id_src2->val) {
    // pc = pc + imm - 4
    rtl_add(&t0, &decinfo.seq_pc, &id_dest->val);
    rtl_subi(&t0, &t0, 4);
    // printf("t0 = 0x%08x\n", t0);
    rtl_j(t0);
  }

  print_asm_template3(beq);
}

make_EHelper(bne) {
  if (id_src->val != id_src2->val) {
    // pc = pc + imm - 4
    rtl_add(&t0, &decinfo.seq_pc, &id_dest->val);
    rtl_subi(&t0, &t0, 4);
    // printf("t0 = 0x%08x\n", t0);
    rtl_j(t0);
  }

  print_asm_template3(bne);
}

make_EHelper(bge) {
  if ((int32_t)id_src->val >= (int32_t)id_src2->val) {
    // pc = pc + imm - 4
    rtl_add(&t0, &decinfo.seq_pc, &id_dest->val);
    rtl_subi(&t0, &t0, 4);
    // printf("t0 = 0x%08x\n", t0);
    rtl_j(t0);
  }

  print_asm_template3(bge);
}

make_EHelper(blt) {
  if ((int32_t)id_src->val < (int32_t)id_src2->val) {
    // pc = pc + imm - 4
    rtl_add(&t0, &decinfo.seq_pc, &id_dest->val);
    rtl_subi(&t0, &t0, 4);
    // printf("t0 = 0x%08x\n", t0);
    rtl_j(t0);
  }

  print_asm_template3(blt);
}

make_EHelper(bltu) {
  if (id_src->val < id_src2->val) {
    // pc = pc + imm - 4
    rtl_add(&t0, &decinfo.seq_pc, &id_dest->val);
    rtl_subi(&t0, &t0, 4);
    // printf("t0 = 0x%08x\n", t0);
    rtl_j(t0);
  }

  print_asm_template3(bltu);
}