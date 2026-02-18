#ifndef __IDT_IA32_H
#define __IDT_IA32_H
// adapted from
// https://github.com/cirosantilli/jamesmolloy-kernel-development-tutorials/blob/3e15384b06726951d21cb6a6173020d4765b4799/5_irq/isr.h#L1
#include <stdint.h>
struct itr_data {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_code, err_code;
  uint32_t eip, cs, eflags;
} __attribute__((packed));

#endif
