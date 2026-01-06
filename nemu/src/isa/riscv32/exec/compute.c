#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(addi) {
  // result = rs1 + imm
  rtl_add(&s0, &id_src->val, &id_src2->val);
  // rd = result
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(addi);
}

make_EHelper(auipc) {
  // result = pc + imm
  rtl_addi(&s0, &decinfo.seq_pc, id_src->imm);
  // rd = result
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template2(auipc);
}