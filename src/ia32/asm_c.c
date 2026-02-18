#include <stdbool.h>
#include <stdint.h>

#include "asm.h"

void clear_interupts(void) { asm("cli"); }

void load_gdt(const struct gdt_header *addr) {
  asm volatile("lgdt %0" ::"m"(*addr));
}

bool is_protected_mode(void) {
  int out;
  asm volatile("mov %%cr0, %0" : "=r"(out));
  return out & 0x1;
}

struct cpuid_data cpuid(uint32_t eax) {
  struct cpuid_data data;
  asm volatile("cpuid\n"
               : "=a"(data.eax), "=b"(data.ebx), "=c"(data.ecx), "=d"(data.edx)
               : "a"(eax)
               : "memory");

  return data;
}

uint64_t read_msr(uint32_t addr) {
  uint32_t lsb = 0, msb = 0;

  asm volatile("rdmsr" : "=a"(lsb), "=d"(msb) : "c"(addr) : "memory");

  return ((uint64_t)msb << 32) | lsb;
}

void write_msr(uint32_t addr, uint64_t val) {
  uint32_t lsb = val & 0xFFFFFFFF, msb = val >> 32;

  asm volatile("wrmsr" ::"c"(addr), "a"(lsb), "d"(msb) : "memory");
}

void outb(uint8_t addr, uint8_t data) {
  asm volatile("outb %b0, %w1" ::"a"(data), "Nd"(addr) : "memory");
}

uint8_t inb(uint8_t addr) {
  uint8_t val;
  asm volatile("inb %w1, %b0" : "=a"(val) : "Nd"(addr) : "memory");
  return val;
}
