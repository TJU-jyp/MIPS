#include "monitor.h"
#include "helper.h"
#include "monitor/watchpoint.h"
#include "reg.h"

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int temu_state = STOP;

void exec(uint32_t);

char assembly[80];
char asm_buf[128];

void print_bin_instr(uint32_t pc) {
	int i;
	int l = sprintf(asm_buf, "%8x:   ", pc);
	for(i = 3; i >= 0; i --) {
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(pc + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 8, "");
}

/* Simulate how the MiniMIPS32 CPU works. */
void cpu_exec(volatile uint32_t n) {
	
	uint32_t pc;
	if(temu_state == END) {
		printf("Program execution has ended. To restart the program, exit TEMU and run again.\n");
		return;
	}
	temu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	for(; n > 0; n --) {

		pc = cpu.pc & 0x1fffffff;  //map the virtual address to the physical address, e.g. high 3 bits in cpu.pc are cleared
		
#ifdef DEBUG
		uint32_t pc_temp = pc;
		if((n & 0xffff) == 0) {
			
			fputc('.', stderr);
		}
#endif

		// 在执行指令前记录当前寄存器状态，用于后续比较
		uint32_t old_reg_values[32];
		for(int i = 1; i < 32; i++) {  // 从1开始，跳过$zero寄存器
			old_reg_values[i] = cpu.gpr[i]._32;
		}
		//在执行 exec 之前备份 PC
		uint32_t current_inst_pc = cpu.pc;

		/* Execute one instruction, including instruction fetch,
		 * instruction decode, and the actual execution. */
		exec(pc);

		// 指令执行后，检查哪些寄存器发生了变化
		for(int i = 1; i < 32; i++) {  // 从1开始，跳过$zero寄存器
			if(cpu.gpr[i]._32 != old_reg_values[i]){
				record_trace(current_inst_pc, i, cpu.gpr[i]._32);
			}
		}

		cpu.pc += 4;

#ifdef DEBUG
		print_bin_instr(pc_temp);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if(n_temp < MAX_INSTR_TO_PRINT) {
			printf("%s\n", asm_buf);
		}
#endif

		/* TODO: check watchpoints here. */


		if(temu_state != RUNNING) { return; }
	}

	if(temu_state == RUNNING) { temu_state = STOP; }
}
