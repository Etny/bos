#ifndef ASM_H
#define ASM_H

#include <stdbool.h>
#include <stdint.h>

#include "gdt.h"

void clear_interupts(void);
void load_gdt(const struct gdt_header *);
void enable_protected_mode(void);
void refresh_cs(void);

bool is_protected_mode(void);

struct cpuid_data {
  uint32_t eax, ebx, ecx, edx;
};

struct cpuid_data cpuid(uint32_t eax);

uint64_t read_msr(uint32_t addr);
void write_msr(uint32_t addr, uint64_t val);

#endif  // !ASM_H
