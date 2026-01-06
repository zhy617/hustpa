#include "cpu/exec.h"

// compute instructions
make_EHelper(lui);
make_EHelper(addi);
make_EHelper(auipc);
make_EHelper(add);
make_EHelper(sltu);
make_EHelper(xor);
make_EHelper(or);
make_EHelper(sltiu);
make_EHelper(sub);
make_EHelper(srli);
make_EHelper(srai);

// control instructions
make_EHelper(jalr);
make_EHelper(jal);
make_EHelper(beq);
make_EHelper(bne);

// load and store instructions
make_EHelper(ld);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);


