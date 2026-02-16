#include "irq.h"

#include <stddef.h>
#include <stdint.h>

#include "asm.h"
#include "panic.h"
#include "print.h"
#include "slice.h"
#include "str.h"

void disable_pics(void);

#define LOCAL_APIC_BASE 0xFEE00000
#define SPURIOUS_INTR_OFFSET 0xF0
#define TIMER_INTR_OFFSET 0x320
#define TIMER_DIV_OFFSET 0x3E0
#define TIMER_CNT_OFFSET 0x380

uint32_t* apic_page = (uint32_t*)LOCAL_APIC_BASE;

static inline void io_wait(void) { outb(0x80, 0); }
static inline uint32_t* apic_register(uint16_t offset) {
  return (uint32_t*)(LOCAL_APIC_BASE + offset);
}

bool check_local_apic(void) { return cpuid(1).edx & (1 << 8); }
bool check_tsc_timer_support(void) { return cpuid(1).ecx & (1 << 24); }

void set_irq_mask(uint16_t offset, bool mask) {
  uint32_t* irq_register = apic_register(offset);
  *irq_register = (*irq_register * (mask << 16));
}

void setup_irqs(void) {
  if (!check_local_apic()) panic("no local APIC support found");

  // We use the LAPIC, so turn off the legacy 8259
  // before it starts sending us random INT 8's again
  disable_pics();

  // We need to set the Software enable bit
  // in the Spurious Interrupt Vector Register
  // (Intel guide 13.4.3 point 2)
  uint32_t* spurious_intr = apic_register(SPURIOUS_INTR_OFFSET);
  *spurious_intr = (*spurious_intr | (1 << 8));

  // *apic_register(TIMER_DIV_OFFSET) = 0x0A;
  // *apic_register(TIMER_INTR_OFFSET) = (1 << 17) | 30;
  // *apic_register(TIMER_CNT_OFFSET) = 1 << 20;

  // We are now ready to receive interupts :)
  asm volatile("sti");
}

#define EOI_OFFSET 0xB0
void eoi(void) { *apic_register(EOI_OFFSET) = 0; }

#define PIC1_CMD_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_CMD_PORT 0xA0
#define PIC2_DATA_PORT 0xA1
#define ICW1_INIT 0x11
#define PIC_CMD_2 0x21
#define ICW4_INIT 0x01

void disable_pics(void) {
  outb(PIC1_CMD_PORT,
       ICW1_INIT);  // Init the PICs. They will now wait for 3 Command Words
  io_wait();
  outb(PIC2_CMD_PORT, ICW1_INIT);
  io_wait();
  outb(PIC1_DATA_PORT, 16);  // Word 1: The offset of their vectors
  io_wait();
  outb(PIC2_DATA_PORT, 16);
  io_wait();
  outb(PIC1_DATA_PORT, 4);  // Word 2: Tell PIC1 that it has a slave on IRQ2,
                            // and PIC2 that is a slave on 2
  io_wait();
  outb(PIC2_DATA_PORT, 2);
  io_wait();
  outb(PIC1_DATA_PORT,
       ICW4_INIT);  // Word 3: Tell the PICs we are running on X86
  io_wait();
  outb(PIC1_DATA_PORT, ICW4_INIT);
  io_wait();

  // Finally, we mask all interrupts
  outb(PIC1_DATA_PORT, 0xFF);
  outb(PIC2_DATA_PORT, 0xFF);
}
