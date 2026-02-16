#include "irq.h"

#include <stddef.h>
#include <stdint.h>

#include "asm.h"
#include "panic.h"
#include "print.h"
#include "str.h"

#define LOCAL_APIC_BASE 0xFEE00000

uint32_t* apic_page = (uint32_t*)LOCAL_APIC_BASE;

bool check_local_apic(void) { return cpuid(1).edx & (1 << 8); }

void setup_irqs(void) {
  if (!check_local_apic()) panic("no local APIC support found");

  uint32_t timer = apic_page[200];
  char buf[20];
  itohex(timer, TO_SLICE(buf));
  print(buf);

  for (size_t i = 0; i < 9; i++) {
    itohex(apic_page[752 + (i * 48)], TO_SLICE(buf));
    print(buf);
  }

  apic_page[200] = timer | 30;
  itohex(apic_page[200], TO_SLICE(buf));
  print(buf);

  uint32_t apic = read_msr(0x1B) & 0xFFFFFF;
  itohex(apic, TO_SLICE(buf));
  print(buf);

  asm volatile("sti");
}
