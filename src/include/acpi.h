#ifndef __ACPI_H
#define __ACPI_H

#include <stdint.h>
struct RSDP {
  char signature[8];
  uint8_t checksum;
  uint64_t oem_id : 48;
  uint8_t revision;
  uintptr_t rsdt_addr;
  uint32_t length;
  uint64_t xsdt_addr;
  uint8_t ext_checksum;
  uint32_t reserverd : 24;
} __attribute__((packed));

extern struct RSDP* rsdp_addr;

void setup_acpi(void);

#endif
