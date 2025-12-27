#include "common.h"

uint32_t dram_read(uint32_t, size_t);
void dram_write(uint32_t, size_t, uint32_t);

/* Memory accessing interfaces */
/* --- 物理内存访问层 (Physical Access) --- */
uint32_t mem_read(uint32_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void mem_write(uint32_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	dram_write(addr, len, data);
}

/* --- 虚拟地址映射层 (Virtual Mapping) --- */
/* 实验要求：虚拟地址最高位清“0”即可映射为物理地址 */

uint32_t vaddr_read(uint32_t addr, size_t len) {
    uint32_t paddr = addr & 0x7FFFFFFF; // 映射逻辑：0x80000000 -> 0x00000000
    return mem_read(paddr, len);       // 调用上面的物理读
}

void vaddr_write(uint32_t addr, size_t len, uint32_t data) {
    uint32_t paddr = addr & 0x7FFFFFFF; // 映射逻辑同上
    mem_write(paddr, len, data);       
}