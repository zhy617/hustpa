#include "cpu/exec.h"

// compute instructions
make_EHelper(lui);
make_EHelper(addi);
make_EHelper(auipc);
make_EHelper(add);
make_EHelper(sltu);

// control instructions
make_EHelper(jalr);
make_EHelper(jal);

// load and store instructions
make_EHelper(ld);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);


