#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EXW(ldi, 1), EXW(ldi, 2), EXW(ld, 4), EMPTY, EXW(ld, 1), EXW(ld, 2), EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EXW(st, 1), EXW(st, 2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry op_imm_1_table [2] = {
  /* b0000000 */ EX(slli),
};

static make_EHelper(op_imm_1) {
  // We only care about bit 0 of funct7 to distinguish slli
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_imm_1_table[0]); break;
    default: exec_inv(pc); break; // Handle unknown funct7
  }
}

// Level 3 table for op-imm, funct3 = 0b101
static OpcodeEntry op_imm_5_table [2] = {
  /* b0000000 */ EX(srli),
  /* b0100000 */ EX(srai),
};

// Level 3 dispatcher for op-imm, funct3 = 0b101
static make_EHelper(op_imm_5) {
  // The repurposed funct7 is in imm[11:5]
  uint32_t funct7 = decinfo.isa.instr.funct7;
  // We only care about bit 5 of this pseudo-funct7
  uint32_t funct7_bit5 = (funct7 >> 5) & 1;
  idex(pc, &op_imm_5_table[funct7_bit5]);
}

static OpcodeEntry op_imm_table [8] = {
  /* b000 */ EX(addi),
  /* b001 */ EX(op_imm_1),
  /* b010 */ EX(slti),
  /* b011 */ EX(sltiu),
  /* b100 */ EX(xori),
  /* b101 */ EX(op_imm_5),
  /* b110 */ EX(ori),
  /* b111 */ EX(andi),
};

static make_EHelper(op_imm) {
  // printf("op_imm funct3 = %d\n", decinfo.isa.instr.funct3);
  idex(pc, &op_imm_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry op_r_0_table [3] = {
  EX(add),
  EX(sub),
  EX(mul), // Added mul
};

static make_EHelper(op_r_0) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_r_0_table[0]); break;
    case 0b0100000: idex(pc, &op_r_0_table[1]); break;
    case 0b0000001: idex(pc, &op_r_0_table[2]); break; 
    default: exec_inv(pc); break; 
  }
}

static OpcodeEntry op_r_1_table [2] = {
  /* b0000000 */ EX(sll),
  /* b0000001 */ EX(mulh), // Added mul
};

static make_EHelper(op_r_1) {
  // We only care about bit 0 of funct7 to distinguish sll/mulh
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_r_1_table[0]); break;
    case 0b0000001: idex(pc, &op_r_1_table[1]); break; // Added mulh case
    default: exec_inv(pc); break; // Handle unknown funct7
  }
}

static OpcodeEntry op_r_2_table [1] = {
  /* b0000000 */ EX(slt),
};

static make_EHelper(op_r_2) {
  // We only care about bit 0 of funct7 to distinguish slt
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_r_2_table[0]); break;
    default: exec_inv(pc); break; // Handle unknown funct7
  }
}

static OpcodeEntry op_r_3_table [2] = {
  /* b0000000 */ EX(sltu),
  /* b0000001 */ EX(mulhu),
};

static make_EHelper(op_r_3) {
  // We only care about bit 0 of funct7 to distinguish sltu/mulhu
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_r_3_table[0]); break;
    case 0b0000001: idex(pc, &op_r_3_table[1]); break; // Added mulhu case
    default: exec_inv(pc); break; // Handle unknown funct7
  }
}

static OpcodeEntry op_r_4_table [2] = {
  /* b0000000 */ EX(xor),
  /* b0000001 */ EX(div),
};

static make_EHelper(op_r_4) {
  // We only care about bit 0 of funct7 to distinguish xor/div
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_r_4_table[0]); break;
    case 0b0000001: idex(pc, &op_r_4_table[1]); break;
    default: exec_inv(pc); break; // Handle unknown funct7
  }
}

static OpcodeEntry op_r_5_table [3] = {
  /* b0000000 */ EX(srl),
  /* b0100000 */ EX(sra),
  /* b0000001 */ EX(divu),
};

static make_EHelper(op_r_5) {
  // We only care about bit 5 of funct7 to distinguish srl/sra
  // uint32_t funct7_bit5 = (decinfo.isa.instr.funct7 >> 5) & 1;
  // idex(pc, &op_r_5_table[funct7_bit5]);
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_r_5_table[0]); break;
    case 0b0100000: idex(pc, &op_r_5_table[1]); break;
    case 0b0000001: idex(pc, &op_r_5_table[2]); break;
    default: exec_inv(pc); break; // Handle unknown funct7
  }
}

static OpcodeEntry op_r_6_table [2] = {
  /* b0000000 */ EX(or),
  /* b0000001 */ EX(rem), // Added mul
};

static make_EHelper(op_r_6) {
  // We only care about bit 0 of funct7 to distinguish or/rem
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_r_6_table[0]); break;
    case 0b0000001: idex(pc, &op_r_6_table[1]); break; // Added rem case
    default: exec_inv(pc); break; // Handle unknown funct7
  }
}

static OpcodeEntry op_r_7_table [2] = {
  /* b0000000 */ EX(and),
  /* b0000001 */ EX(remu), 
};

static make_EHelper(op_r_7) {
  switch (decinfo.isa.instr.funct7) {
    case 0b0000000: idex(pc, &op_r_7_table[0]); break;
    case 0b0000001: idex(pc, &op_r_7_table[1]); break; // Added remu case
    default: exec_inv(pc); break; // Handle unknown funct7
  }
}

static OpcodeEntry op_r_table [8] = {
  /* b000 */ EX(op_r_0),
  /* b001 */ EX(op_r_1),
  /* b010 */ EX(op_r_2),
  /* b011 */ EX(op_r_3),
  /* b100 */ EX(op_r_4),
  /* b101 */ EX(op_r_5),
  /* b110 */ EX(op_r_6),
  /* b111 */ EX(op_r_7),
};

static make_EHelper(op_r) {
  // printf("op_r funct3 = %d\n", decinfo.isa.instr.funct3);
  idex(pc, &op_r_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry op_b_table [8] = {
  /* b000 */ EX(beq),
  /* b001 */ EX(bne),
  /* b010 */ EMPTY,
  /* b011 */ EMPTY,
  /* b100 */ EX(blt),
  /* b101 */ EX(bge),
  /* b110 */ EX(bltu),
  /* b111 */ EX(bgeu),
};

static make_EHelper(op_b) {
  // printf("op_b funct3 = %d\n", decinfo.isa.instr.funct3);
  idex(pc, &op_b_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry system_table [8] = {
  /* b000 */ EMPTY, // ecall/ebreak
  /* b001 */ EMPTY,         // csrrw
  /* b010 */ EX(csrrs),     // csrrs
  /* b011 */ EMPTY,         // csrrc
  /* b100 */ EMPTY,
  /* b101 */ EMPTY,         // csrrwi
  /* b110 */ EMPTY,         // csrrsi
  /* b111 */ EMPTY,         // csrrci
};

static make_EHelper(system) {
  printf("system funct3 = %d\n", decinfo.isa.instr.funct3);
  idex(pc, &system_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00000 */ IDEX(ld, load),    EMPTY,              EMPTY,            EMPTY,
  /* b00100 */ IDEX(I, op_imm),   IDEX(U, auipc),     EMPTY,            EMPTY,
  /* b01000 */ IDEX(st, store),   EMPTY,              EMPTY,            EMPTY,
  /* b01100 */ IDEX(R, op_r),     IDEX(U, lui),       EMPTY,            EMPTY,
  /* b10000 */ EMPTY,             EMPTY,              EMPTY,            EMPTY,
  /* b10100 */ EMPTY,             EMPTY,              EMPTY,            EMPTY,
  /* b11000 */ IDEX(B, op_b),     IDEX(I, jalr),      EX(nemu_trap),    IDEX(J, jal),
  /* b11100 */ IDEX(I, system),   EMPTY,              EMPTY,            EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  // printf("0x%08x: 0x%08x fuck \n", *pc - 4, decinfo.isa.instr.val);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  // printf("opcode6_2: %d\n", decinfo.isa.instr.opcode6_2);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
