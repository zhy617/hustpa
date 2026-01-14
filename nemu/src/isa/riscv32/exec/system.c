#include "cpu/exec.h"
#include "isa/reg.h"
#include "isa/intr.h"

make_EHelper(csrrs) {
  // 1. Read the old value from CSR and store it in a temporary RTL register s1.
  rtl_li(&s1, csr_read(id_src2->val));

  // 2. Write the old value (now in s1) to the destination register rd.
  rtl_sr(id_dest->reg, &s1, 4);

  // 3. Calculate the new value: new_val = old_val | rs1_val
  //    rs1_val is in id_src->val, old_val is in s1. Result stored in s0.
  rtl_or(&s0, &s1, &id_src->val);

  // 4. Write the new value (in s0) back to the CSR.
  csr_write(id_src2->val, s0);
  // printf("pc = %x, csr addr = %x, old csr val = %x, rs1 val = %x, new csr val = %x, dest val = %x\n",
  //        decinfo.seq_pc, id_src2->val, s1, id_src->val, s0, id_dest->val);
  // printf("dest addr = %p\n", &(id_dest->val));
  print_asm_template3(csrrs);
}

make_EHelper(csrrw) {
    // 1. Read the old value from CSR and store it in a temporary RTL register s1.
    // printf("imm: %d\n", id_src2->val);
    rtl_li(&s1, csr_read(id_src2->val));
    
    // 2. Write the old value (now in s1) to the destination register rd.
    rtl_sr(id_dest->reg, &s1, 4);
    
    // 3. Write the value from rs1 (id_src->val) directly to the CSR.
    csr_write(id_src2->val, id_src->val);
    
    // printf("Fuckyou\n");
    // printf("cpu.sepc before = %x\n", cpu.sepc);
    // printf("pc = %x, csr addr = %x, old csr val = %x, rs1 val = %x, new csr val = %x, dest val = %x\n",
    //        decinfo.seq_pc, id_src2->val, s1, id_src->val, id_src->val, s1);
    // printf("dest reg = %p\n", id_dest->reg);

    print_asm_template3(csrrw);
}

make_EHelper(ecall) {
  // Trigger an environment call exception
  // make sure not sret
  assert((decinfo.isa.instr.val & 0xfff) == 0x73);
  // printf("ecall at pc = %x\n", decinfo.seq_pc);
  // printf("cpu.pc before = %x\n", cpu.pc);
  raise_intr(8, cpu.pc); // 8 is the exception code for ecall from U-mode

  print_asm("ecall");
}

make_EHelper(sret) {
  rtl_j(cpu.sepc); 
  
  print_asm("sret");
}