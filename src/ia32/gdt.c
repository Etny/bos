#include "gdt.h"
#include "asm.h"

#define SEG_DESC(base, limit, flags)                                           \
  (((((uint64_t)limit & 0x000F0000) | ((flags << 8) & 0x00F0FF00) |            \
     ((base >> 16) & 0x000000FF) | (base & 0xFF000000))                        \
    << 32) |                                                                   \
   (base << 16) | (limit & 0x0000FFFF))

#define GDT_NULL_SEG 0
#define GDT_KERNEL_CODE_SEG 1
#define GDT_KERNEL_DATA_SEG 2

#define FLAGS_KERNEL_CODE (uint16_t)((0b1100 << 12) | 0b10011011)
#define FLAGS_KERNEL_DATA (uint16_t)((0b1100 << 12) | 0b10010011)

struct gdt boot_gdt __attribute__((section(".boot_gdt"))) = {
    .gdt = {[GDT_NULL_SEG] = SEG_DESC(0, 0, 0),
            [GDT_KERNEL_CODE_SEG] = SEG_DESC(0, 0x000FFFFF, FLAGS_KERNEL_CODE),
            [GDT_KERNEL_DATA_SEG] =
                SEG_DESC(0, 0x000FFFFF, FLAGS_KERNEL_DATA)}};

void setup_gdt(void) {
  struct gdt_desc desc = {
      .base = (uint32_t)&boot_gdt,
      .limit = sizeof(struct gdt) - 1,
  };
  load_gdt(&desc);
  refresh_cs();
}
