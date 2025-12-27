#ifndef __MONITOR_H__
#define __MONITOR_H__
#include <stdint.h>
#include <stdio.h>

enum { STOP, RUNNING, END };
extern int temu_state;
extern FILE *trace_fp;

//添加 trace 相关函数声明
void init_trace();
void close_trace();
void record_trace(uint32_t pc, int reg_no, uint32_t reg_val);

#endif
