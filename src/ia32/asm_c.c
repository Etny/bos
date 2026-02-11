#include <stdbool.h>

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
