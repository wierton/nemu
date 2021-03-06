#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr mov

void init_tlb();

static void do_execute() {
	OPERAND_W(op_dest, op_src->val, R_DS);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX), R_DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr, R_DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE == 4

make_helper(mov_cr2r_l)
{
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch(m.reg)
	{
		case 0:reg_l(m.R_M) = cpu.CR0.val;break;
		case 3:reg_l(m.R_M) = cpu.CR3.val;break;
	}
	
	print_asm("mov %%cr%d,%%%s", m.reg, regsl[m.R_M]);
	return 2;
}

make_helper(mov_r2cr_l)
{
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch(m.reg)
	{
		case 0:cpu.CR0.val = reg_l(m.R_M);break;
		case 3:
			   init_tlb();
			   cpu.CR3.val = reg_l(m.R_M);
			   break;
	}
	print_asm("mov %%%s,%%cr%d", regsl[m.R_M], m.reg);
	return 2;
}

make_helper(mov_r2sreg)
{
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	cpu.gsreg[m.reg].val = reg_l(m.R_M);
	return 2;
}

#endif

#include "cpu/exec/template-end.h"
