
#ifndef GDT_H
#define GDT_H

#include <stdint.h>

void setup_gdt(void);

struct gdt {
  uint64_t gdt[32];
} __attribute__((packed));

struct gdt_desc {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

#endif // !GDT_H
