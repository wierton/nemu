#include "cpu/exec/template-start.h"

#define instr movs

void ExecLog();

make_helper(concat(movs_,SUFFIX))
{
	if(eip>=0xc01013a8)
		ExecLog();
	int IncDec = cpu.DF?-DATA_BYTE:DATA_BYTE;
	DATA_TYPE src = MEM_R(cpu.esi, R_DS);
	MEM_W(cpu.edi, src, R_ES);
	cpu.edi += IncDec;
	cpu.esi += IncDec;

	print_asm("movs"str(SUFFIX)" $es:($esi),$ds:($edi)");
	return 1;
}

#include "cpu/exec/template-end.h"
