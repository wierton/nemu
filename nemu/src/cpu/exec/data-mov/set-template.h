#include "cpu/exec/template-start.h"

#define instr set

static void do_eflags_execute(bool flag,int diplacement,char set_flagstr[])
{
	if(flag)
		OPERAND_W(op_src,1,R_DS);
	else
		OPERAND_W(op_src,0, R_DS);
	print_asm("%s %s",set_flagstr,op_src->str);
}


#define flagstr seta
make_eflags_helper(rm,!cpu.CF,!cpu.ZF)
#undef flagstr

#define flagstr setae
make_eflags_helper(rm,!cpu.CF,1)
#undef flagstr

#define flagstr setb
make_eflags_helper(rm,cpu.CF,1)
#undef flagstr

#define flagstr setbe
make_eflags_helper(rm,cpu.CF || cpu.ZF,1)
#undef flagstr

#define flagstr setc
make_eflags_helper(rm,cpu.CF,1)
#undef flagstr

#define flagstr sete
make_eflags_helper(rm,cpu.ZF,1)
#undef flagstr

#define flagstr setz
make_eflags_helper(rm,cpu.ZF,1)
#undef flagstr

#define flagstr setg
make_eflags_helper(rm,!cpu.ZF,cpu.SF==cpu.OF)
#undef flagstr

#define flagstr setge
make_eflags_helper(rm,cpu.SF==cpu.OF,1)
#undef flagstr

#define flagstr setl
make_eflags_helper(rm,cpu.SF!=cpu.OF,1)
#undef flagstr

#define flagstr setle
make_eflags_helper(rm,(cpu.ZF || (cpu.SF!=cpu.OF)),1)
#undef flagstr

#define flagstr setna
make_eflags_helper(rm,cpu.CF,cpu.SF)
#undef flagstr

#define flagstr setnae
make_eflags_helper(rm,cpu.CF,1)
#undef flagstr

#define flagstr setnb
make_eflags_helper(rm,!cpu.CF,1)
#undef flagstr

#define flagstr setnbe
make_eflags_helper(rm,!cpu.ZF,!cpu.ZF)
#undef flagstr

#define flagstr setnc
make_eflags_helper(rm,!cpu.CF,1)
#undef flagstr

#define flagstr setne
make_eflags_helper(rm,!cpu.ZF,1)
#undef flagstr

#define flagstr setng
make_eflags_helper(rm,cpu.ZF || (cpu.SF!=cpu.OF),1)
#undef flagstr

#define flagstr setnge
make_eflags_helper(rm,cpu.SF!=cpu.OF,1)
#undef flagstr

#define flagstr setnl
make_eflags_helper(rm,cpu.SF==cpu.OF,1)
#undef flagstr

#define flagstr setnle
make_eflags_helper(rm,!cpu.ZF,cpu.SF==cpu.OF)
#undef flagstr

#define flagstr setno
make_eflags_helper(rm,!cpu.OF,1)
#undef flagstr

#define flagstr setnp
make_eflags_helper(rm,!cpu.PF,1)
#undef flagstr

#define flagstr setns
make_eflags_helper(rm,!cpu.SF,1)
#undef flagstr

#define flagstr setnz
make_eflags_helper(rm,!cpu.ZF,1)
#undef flagstr

#define flagstr seto
make_eflags_helper(rm,cpu.OF,1)
#undef flagstr

#define flagstr setp
make_eflags_helper(rm,cpu.PF,1)
#undef flagstr

#define flagstr setpe
make_eflags_helper(rm,cpu.PF,1)
#undef flagstr

#define flagstr setpo
make_eflags_helper(rm,!cpu.PF,1)
#undef flagstr

#define flagstr sets
make_eflags_helper(rm,cpu.SF,1)
#undef flagstr


#include "cpu/exec/template-end.h"
	
