#include "asm.h"

void clear_interupts(void) { asm("cli"); }

void load_gdt(const struct gdt_header *addr) {
  asm volatile("lgdt %0" ::"m"(*addr));
}
