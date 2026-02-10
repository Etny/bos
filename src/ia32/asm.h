#ifndef ASM_H
#define ASM_H

#include <stdint.h>

#include "gdt.h"

void clear_interupts(void);
void load_gdt(const struct gdt_header *);
void enable_protected_mode(void);
void refresh_cs(void);

#endif  // !ASM_H
