#include "../include/monitor/monitor.h"

void init_monitor(int, char *[]);
void restart();
void ui_mainloop();

int main(int argc, char *argv[]) {

	/* Initialize the monitor. */
	init_monitor(argc, argv);

	/* Initialize golden trace */
	init_trace();

	/* Test the implementation of the `CPU_state' structure. */
	//reg_test();

	/* Initialize the virtual computer system. */
	restart();

	/* Receive commands from user. */
	ui_mainloop();

	/* Close golden trace */
	close_trace();

	return 0;
}
