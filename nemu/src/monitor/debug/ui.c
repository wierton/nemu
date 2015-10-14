#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },

	/* TODO: Add more commands */
	{ "si", "Execute the program x steps",cmd_si},
	{ "info", "Print the reg or watch_point",cmd_info},
	{ "x", "Read memory from given address",cmd_x},
	{ "p", "Calculate the expression",cmd_p},
	{ "w", "Add a watchpoint",cmd_w},
	{ "d", "Delete a watchpoint",cmd_d}
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si(char *args)
{
	int steps;
	bool is_valid;
	
	if(args == NULL)
	{
		cpu_exec(1);
		return 0;
	}
	
	steps = eval(args, &is_valid);
	assert(is_valid);
	cpu_exec(steps);
	return 0;
}

static int cmd_info(char *args)
{

	if(strcmp(args,"r")==0)
	{
		printf("eax\t$0x%08x\t%d\n",cpu.eax,cpu.eax);
		printf("ecx\t$0x%08x\t%d\n",cpu.ecx,cpu.ecx);
		printf("edx\t$0x%08x\t%d\n",cpu.edx,cpu.edx);
		printf("ebx\t$0x%08x\t%d\n",cpu.ebx,cpu.ebx);
		printf("esp\t$0x%08x\t$0x%08x\n",cpu.esp,cpu.esp);
		printf("ebp\t$0x%08x\t$0x%08x\n",cpu.ebp,cpu.ebp);
		printf("esi\t$0x%08x\t%d\n",cpu.esi,cpu.esi);
		printf("edi\t$0x%08x\t%d\n",cpu.edi,cpu.edi);
		printf("eip\t$0x%08x\t$0x%08x\n",cpu.eip,cpu.eip);
	}
	else if(strcmp(args,"w")==0)
	{
		print_wp();
	}
	else if(strcmp(args,"f")==0)
	{
		printf("OF SF ZF PF CF\n");
		printf("%d  %d  %d  %d  %d\n", cpu.OF, cpu.SF, cpu.ZF, cpu.PF, cpu.CF);
	}
	else
		printf("Unknown parameter:%s\n",args);
	return 0;
}

static int cmd_x(char *args)
{
	int i=0;
	char ch,*para, *expression;
	int len_para, len_args;
	size_t len;
	uint32_t read_addr = 0;
	bool para_success = false, addr_success = false;

	if(args == NULL)
		return 0;

	len_args = strlen(args);

	para = strtok(args, " ");
	assert(para != NULL);
	len_para = strlen(para);
	assert(args + len_args > para + len_para);

	expression = strtok(NULL, " ");
		
	read_addr = eval(expression, &addr_success);

	if(strcmp(para, "s")==0)
	{
		printf("0x%8x:",read_addr);
		do
		{
			ch = swaddr_read(read_addr+i,1);
			printf("%c",ch);
			i++;
		}while(ch != 0);
		printf("\n");
		return 0;
	}

	len = eval(para, &para_success);
	assert(para_success && addr_success);

	for(i=0;i<len;i++)
	{
		uint32_t value = swaddr_read(read_addr+4*i,4);
		printf("0x%0x\t0x%0x\t%u\n",read_addr+4*i,value,value);
	}
		
	return 0;
}

static int cmd_p(char *args)
{
	int result = 0;
	bool is_success = true;
	result = eval(args,&is_success);
	if(is_success)
		printf("%d\n",result);
	else
		printf("Invalid Expression!\n");
	return 0;
}

static int cmd_w(char *args)
{
	int value;
	size_t len = strlen(args);
	WP *wp;
	bool is_valid;
	value = eval(args, &is_valid);
	if(is_valid)
	{
		wp = new_wp();
		if(len <= WP_EXPR-1)
		{
			strncpy(wp->expr, args, len);
			wp->value = value;
		}
		else
			printf("Expression is too long!\n");
	}
	return 0;
}

static int cmd_d(char *args)
{
	int no;
	bool is_valid;
	no = eval(args,&is_valid);
	if(is_valid)
		free_wp(find_wp(no));
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
