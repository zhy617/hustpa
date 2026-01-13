#include "nemu.h"

const char *regsl[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  int i;
  for (i = 0; i < 32; i++) {
    printf("%s\t0x%08x\t%u\n", reg_name(i, 4), reg_l(i), reg_l(i));
  }
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
  if (strcmp(s, "pc") == 0) {
    *success = true;
    return cpu.pc;
  }
  int i;
  for (i = 0; i < 32; i++) {
    if (strcmp(s, reg_name(i, 4)) == 0) {
      *success = true;
      return reg_l(i);
    }
  }
  *success = false;
  return 0;
}

#define CSR_SEPC   0x141
#define CSR_SCAUSE 0x142
#define CSR_STVEC  0x105
#define CSR_SSTATUS 0x100


rtlreg_t csr_read(uint32_t addr) {
  switch (addr) {
    case CSR_SEPC:   return cpu.sepc;
    case CSR_SCAUSE: return cpu.scause;
    case CSR_STVEC:  return cpu.stvec;
    case CSR_SSTATUS:return cpu.sstatus;
    default:
      // 处理未实现的CSR读取
      panic("CSR read to unimplemented address 0x%x", addr);
      return 0;
  }
}

rtlreg_t* csr_read_addr(uint32_t addr) {
  switch (addr) {
    case CSR_SEPC:   return &cpu.sepc;
    case CSR_SCAUSE: return &cpu.scause;
    case CSR_STVEC:  return &cpu.stvec;
    case CSR_SSTATUS:return &cpu.sstatus;
    default:
      // 处理未实现的CSR读取
      panic("CSR read to unimplemented address 0x%x", addr);
      return NULL;
  }
}

void csr_write(uint32_t addr, rtlreg_t data) {
  switch (addr) {
    case CSR_SEPC:   cpu.sepc = data; break;
    case CSR_SCAUSE: cpu.scause = data; break;
    case CSR_STVEC:  cpu.stvec = data; break;
    case CSR_SSTATUS:cpu.sstatus = data; break;
    default:
      // 处理未实现的CSR写入
      panic("CSR write to unimplemented address 0x%x", addr);
  }
}