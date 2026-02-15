
#include "idt.h"

#include <stddef.h>
#include <stdint.h>

#include "./gdt_dec.h"
#include "itr_dec.h"
#include "panic.h"
#include "terminal.h"

#define DPL_KERNEL 0
#define GATE_TYPE_TASK      0b0101
#define GATE_TYPE_INT_16    0b0110
#define GATE_TYPE_TRAP_16   0b0111
#define GATE_TYPE_INTR_32   0b1110
#define GATE_TYPE_TRAP_32   0b1111

#define GATE_FLAGS(dpl, type) (0 | (1 << 7) | ((dpl & 0b11) << 5) | (type & 0b1111))

#define GATE_FLAGS_TRAP_32 GATE_FLAGS(DPL_KERNEL, GATE_TYPE_TRAP_32)
#define GATE_FLAGS_INTR_32 GATE_FLAGS(DPL_KERNEL, GATE_TYPE_INTR_32)

struct gate_desc {
  uint16_t offset_lower;
  uint16_t seg_selector;
  uint8_t _zero;
  uint8_t flags;
  uint16_t offset_upper;
} __attribute__((packed));

#define IDT_ENTRIES 22

struct gate_desc boot_idt[IDT_ENTRIES];

struct idt_header {
  uint16_t size;
  uintptr_t offset;
} __attribute__((packed));

static struct idt_header header;

extern void* itr_stub_table[IDT_ENTRIES];

void set_idt_entry(size_t idx, uint8_t flags) {
  struct gate_desc* entry = &boot_idt[idx];

  entry->_zero = 0;
  entry->flags = flags;
  entry->seg_selector = GDT_KERNEL_CODE_SEG_IDX;
  entry->offset_lower = (uintptr_t)itr_stub_table[idx] & 0x0000FFFF;
  entry->offset_upper = (uintptr_t)itr_stub_table[idx] >> 16;
}

void setup_idt(void) {
  for (size_t i = 0; i < IDT_ENTRIES; i++) set_idt_entry(i, GATE_FLAGS_TRAP_32);

  header.size = (uint16_t)sizeof(struct gate_desc) * 32 - 1;
  header.offset = (uintptr_t)&boot_idt;

  asm volatile("lidt %0" ::"m"(header));
  asm volatile("sti");
  // asm volatile("int $3");
  //

  term_writeline("idt setup complete");
}
