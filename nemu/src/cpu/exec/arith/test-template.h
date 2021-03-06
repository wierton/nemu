#include "cpu/exec/template-start.h"


#define instr test

static void do_execute() {
	DATA_TYPE dst = op_src->val & op_dest->val;
	cpu.OF = 0;
	cpu.SF = MSB(dst);
	cpu.ZF = (dst == 0);
	cpu.PF = anyEvenBit(dst);
	cpu.CF = 0;
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
