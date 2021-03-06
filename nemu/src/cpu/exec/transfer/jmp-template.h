#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	DATA_TYPE_S result = op_src->val;
	cpu.eip += result;
	print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(jmp_rm_, SUFFIX))
{
	int len = concat(decode_rm_,SUFFIX)(eip+1);
	cpu.eip = op_src->val - (len+1);
	print_asm_template1();
	return len+1;

}
#endif
make_instr_helper(i)

#if DATA_BYTE == 4
make_helper(jmp_ptr_l)
{
	int len = 7;
	cpu.eip = instr_fetch(eip + 1, 4) - len;
	cpu.CS.val = instr_fetch(eip + 5, 2);
	print_asm("ljmp $0x%x,$0x%x", cpu.CS.val, cpu.eip + len);
	return len;
}
#endif

#include "cpu/exec/template-end.h"
