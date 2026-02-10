#include "gdt.h"

#include "asm.h"

struct gdt_entry {
  uint16_t limit_lower : 16;
  uint32_t base_lower : 24;
  uint8_t access : 8;
  uint8_t limit_upper : 4;
  uint8_t flags : 4;
  uint8_t base_upper : 8;
} __attribute__((packed));

struct gdt {
  struct gdt_entry gdt[32];
} __attribute__((packed));

struct gdt_header {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

#define SEG_DESC(base, limit, flagbits)     {                   \
    .limit_lower    =   ((limit     >> 0)   & 0x0000FFFF),      \
    .base_lower     =   ((base      >> 0)   & 0x00FFFFFF),      \
    .access         =   ((flagbits  >> 0)   & 0x000000FF),      \
    .limit_upper    =   ((limit     >> 16)  & 0x0000000F),      \
    .flags          =   ((flagbits  >> 12)  & 0x0000000F),      \
    .base_upper     =   ((base      >> 24)  & 0x000000FF),      \
}

#define GDT_NULL_SEG        0
#define GDT_KERNEL_CODE_SEG 1
#define GDT_KERNEL_DATA_SEG 2

#define FLAGS_KERNEL_CODE (uint16_t)((0b1100 << 12) | 0b10011011)
#define FLAGS_KERNEL_DATA (uint16_t)((0b1100 << 12) | 0b10010011)

struct gdt boot_gdt __attribute__((section(".boot_gdt"))) = {
    .gdt = {
        [GDT_NULL_SEG] = SEG_DESC(0, 0, 0),
        [GDT_KERNEL_CODE_SEG] = SEG_DESC(0, 0x000FFFFF, FLAGS_KERNEL_CODE),
        [GDT_KERNEL_DATA_SEG] = SEG_DESC(0, 0x000FFFFF, FLAGS_KERNEL_DATA)}};

void setup_gdt(void) {
  struct gdt_header desc = {
      .base = (uint32_t)((uint64_t)&boot_gdt & 0xFFFFFFFF),
      .limit = sizeof(struct gdt) - 1,
  };
  load_gdt(&desc);
  refresh_cs();
}
