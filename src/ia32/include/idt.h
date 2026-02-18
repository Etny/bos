#ifndef __IDT_H
#define __IDT_H

#include <stdint.h>
struct itr_data {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_code, err_code;
  uint32_t eip, cs, eflags;
} __attribute__((packed));
void setup_idt(void);

extern void (*irq_table[224])(uint32_t int_code);

#endif  // !__IDT_H
