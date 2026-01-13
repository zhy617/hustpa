#include "cpu/exec.h"
#include "isa/reg.h"

make_EHelper(csrrs) {
  // 1. Read the old value from CSR and store it in a temporary RTL register s1.
  rtl_li(&s1, csr_read(id_src->imm));

  // 2. Write the old value (now in s1) to the destination register rd.
  rtl_mv(&id_dest->val, &s1);

  // 3. Calculate the new value: new_val = old_val | rs1_val
  //    rs1_val is in id_src->val, old_val is in s1. Result stored in s0.
  rtl_or(&s0, &s1, &id_src->val);

  // 4. Write the new value (in s0) back to the CSR.
  csr_write(id_src->imm, s0);

  print_asm_template3(csrrs);
}

make_EHelper(csrrw) {
    // 1. Read the old value from CSR and store it in a temporary RTL register s1.
    printf("imm: %d\n", id_src->imm);
    rtl_li(&s1, csr_read(id_src->imm));
    
    // 2. Write the old value (now in s1) to the destination register rd.
    rtl_mv(&id_dest->val, &s1);
    
    // 3. Write the value from rs1 (id_src->val) directly to the CSR.
    csr_write(id_src->imm, id_src->val);
    
    print_asm_template3(csrrw);
}