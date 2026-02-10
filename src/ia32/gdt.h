
#ifndef GDT_H
#define GDT_H

#include <stdint.h>

void setup_gdt(void);

struct gdt_header;

#endif  // !GDT_H
