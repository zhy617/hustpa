#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EMPTY, EMPTY, EXW(ld, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EMPTY, EMPTY, EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry op_imm_table [8] = {
  /* b000 */ EX(addi),
  /* b001 */ EMPTY,
  /* b010 */ EMPTY,
  /* b011 */ EMPTY,
  /* b100 */ EMPTY,
  /* b101 */ EMPTY,
  /* b110 */ EMPTY,
  /* b111 */ EMPTY,
};

static make_EHelper(op_imm) {
  idex(pc, &op_imm_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry auipc_table [1] = {
  EX(auipc),
};


static OpcodeEntry opcode_table [32] = {
  /* b00000 */ IDEX(ld, load),    EMPTY,              EMPTY,            EMPTY,
  /* b00100 */ IDEX(I, op_imm),   IDEX(U, auipc),     EMPTY,            EMPTY,
  /* b01000 */ IDEX(st, store),   EMPTY,              EMPTY,            EMPTY,
  /* b01100 */ EMPTY,             IDEX(U, lui),       EMPTY,            EMPTY,
  /* b10000 */ EMPTY,             EMPTY,              EMPTY,            EMPTY,
  /* b10100 */ EMPTY,             EMPTY,              EMPTY,            EMPTY,
  /* b11000 */ EMPTY,             IDEX(I, jalr),      EMPTY,            IDEX(J, jal),
  /* b11100 */ EMPTY,             EMPTY,              EX(nemu_trap),    EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
