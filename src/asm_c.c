#include "asm.h"

void clear_interupts(void) { asm("cli"); }

void load_gdt(const struct gdt_desc *addr) {
  asm volatile("lgdt %0" ::"m"(*addr));
}

// void enable_protected_mode(void) {
//   asm volatile("mov %cr0, %eax\n"
//                "orb  $0, %al\n"
//                "mov %eax, %cr0");
// }
