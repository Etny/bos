
#ifndef GDT_H
#define GDT_H

#include <stdint.h>

void setup_gdt(void);

struct gdt_header {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

#endif  // !GDT_H
