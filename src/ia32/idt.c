
#include "idt.h"

#include <stddef.h>
#include <stdint.h>

#include "./gdt_dec.h"
#include "panic.h"

#define DPL_KERNEL 0
#define GATE_TYPE_TASK      0b0101
#define GATE_TYPE_INT_16    0b0110
#define GATE_TYPE_TRAP_16   0b0111
#define GATE_TYPE_INT_32    0b1110
#define GATE_TYPE_TRAP_32   0b1111

#define GATE_FLAGS(dpl, type) (0 | (1 << 7) | ((dpl & 0b11) << 5) | (type & 0b1111))

#define GATE_FLAGS_TRAP_32 GATE_FLAGS(DPL_KERNEL, GATE_TYPE_INT_32)

#define GATE_DESC(offset, segment, flagbits)  {             \
    .offset_lower   =   (((uint32_t)offset    >> 0)   & 0x0000FFFF),  \
    .seg_selector   =   ((segment   >> 0)   & 0x0000FFFF),  \
    .offset_upper   =   (((uint32_t)offset    >> 16)  & 0x0000FFFF),  \
    ._zero          =   0,                                  \
    .flags          = flagbits,                             \
}

struct gate_desc {
  uint16_t offset_lower;
  uint16_t seg_selector;
  uint8_t _zero;
  uint8_t flags;
  uint16_t offset_upper;
} __attribute__((packed));

struct idt {
  struct gate_desc entries[256];
} __attribute__((packed));

#define DE_IDX 0

void div_zero_trap(void) { panic("Div by zero :("); }
void except_base(void) { panic("Generic error handler :)"); }

struct gate_desc boot_idt[32];

struct idt_header {
  uint16_t size;
  uintptr_t offset;
} __attribute__((packed));

static struct idt_header header;

extern void* intr_stub_table[32];

void setup_idt(void) {
  for (int idx = 0; idx < 32; idx++) {
    struct gate_desc DE_DESC = GATE_DESC(intr_stub_table[idx], 0x08, 0x8E);
    boot_idt[idx].offset_upper = DE_DESC.offset_upper;
    boot_idt[idx].offset_lower = DE_DESC.offset_lower;
    boot_idt[idx]._zero = DE_DESC._zero;
    boot_idt[idx].flags = DE_DESC.flags;
    boot_idt[idx].seg_selector = DE_DESC.seg_selector;
  }

  header.size = (uint16_t)sizeof(struct gate_desc) * 32 - 1;
  header.offset = (uintptr_t)&boot_idt;

  asm volatile("lidt %0" ::"m"(header));
  asm volatile("sti");

  // except_base();
}
