#include "cpu/exec.h"

make_EHelper(lui) {
  // printf("fuck you\n");
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

make_EHelper(add) {
  rtl_add(&s0, &id_src->val, &id_src2->val);
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(add);
}

make_EHelper(auipc) {
  // result = pc + imm
  rtl_addi(&s0, &decinfo.seq_pc, id_src->imm);
  // rd = result
  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template2(auipc);
}

make_EHelper(sltu) {
  rtl_setrelop(RELOP_LTU, &s0, &id_src->val, &id_src2->val);

  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(sltu);
}

make_EHelper(xor) { 
  rtl_xor(&s0, &id_src->val, &id_src2->val);

  rtl_sr(id_dest->reg, &s0, 4);

  print_asm_template3(xor);
}