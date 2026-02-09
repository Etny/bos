#ifndef ASM_H
#define ASM_H

#include "gdt.h"
#include <stdint.h>

void clear_interupts(void);
void load_gdt(const struct gdt_desc *);
void enable_protected_mode(void);
void refresh_cs(void);

#endif // !ASM_H
