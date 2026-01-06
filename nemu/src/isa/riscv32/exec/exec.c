#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EMPTY, EMPTY, EXW(ld, 4), EMPTY, EXW(ld, 1), EMPTY, EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EMPTY, EXW(st, 2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
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
  /* b001 */ EMPTY,
  /* b010 */ EMPTY,
  /* b011 */ EX(sltiu),
  /* b100 */ EMPTY,
  /* b101 */ EX(op_imm_5),
  /* b110 */ EMPTY,
  /* b111 */ EX(andi),
};

static make_EHelper(op_imm) {
  // printf("op_imm funct3 = %d\n", decinfo.isa.instr.funct3);
  idex(pc, &op_imm_table[decinfo.isa.instr.funct3]);
}

// Level 3 table for funct3 = 0b000, indexed by funct7
static OpcodeEntry op_r_0_table [2] = {
  /* b0000000 */ EX(add),
  /* b0100000 */ EX(sub),
};

// Level 3 dispatcher for funct3 = 0b000
static make_EHelper(op_r_0) {
  // We only care about bit 5 of funct7 to distinguish add/sub
  uint32_t funct7_bit5 = (decinfo.isa.instr.funct7 >> 5) & 1;
  idex(pc, &op_r_0_table[funct7_bit5]);
}

static OpcodeEntry op_r_table [8] = {
  /* b000 */ EX(op_r_0),
  /* b001 */ EMPTY,
  /* b010 */ EMPTY,
  /* b011 */ EX(sltu),
  /* b100 */ EX(xor),
  /* b101 */ EMPTY,
  /* b110 */ EX(or),
  /* b111 */ EMPTY,
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
  /* b100 */ EMPTY,
  /* b101 */ EMPTY,
  /* b110 */ EMPTY,
  /* b111 */ EMPTY,
};

static make_EHelper(op_b) {
  // printf("op_b funct3 = %d\n", decinfo.isa.instr.funct3);
  idex(pc, &op_b_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00000 */ IDEX(ld, load),    EMPTY,              EMPTY,            EMPTY,
  /* b00100 */ IDEX(I, op_imm),   IDEX(U, auipc),     EMPTY,            EMPTY,
  /* b01000 */ IDEX(st, store),   EMPTY,              EMPTY,            EMPTY,
  /* b01100 */ IDEX(R, op_r),     IDEX(U, lui),       EMPTY,            EMPTY,
  /* b10000 */ EMPTY,             EMPTY,              EMPTY,            EMPTY,
  /* b10100 */ EMPTY,             EMPTY,              EMPTY,            EMPTY,
  /* b11000 */ IDEX(B, op_b),     IDEX(I, jalr),      EX(nemu_trap),    IDEX(J, jal),
  /* b11100 */ EMPTY,             EMPTY,              EMPTY,    EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  // printf("0x%08x: 0x%08x fuck \n", *pc - 4, decinfo.isa.instr.val);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  // printf("opcode6_2: %d\n", decinfo.isa.instr.opcode6_2);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
