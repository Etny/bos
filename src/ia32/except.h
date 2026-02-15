#ifndef __ERROR_H
#define __ERROR_H

#include <stdint.h>

// adapted from
// https://github.com/cirosantilli/jamesmolloy-kernel-development-tutorials/blob/3e15384b06726951d21cb6a6173020d4765b4799/5_irq/isr.h#L1
struct registers {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_code, err_code;
  uint32_t userebp, eip, cs, eflags, useresp, ss;
} __attribute__((packed));

// The default exception handler
void exception_handler(struct registers *reg);

#endif
