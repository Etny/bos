#include "acpi.h"

#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>

#include "irq_ia32.h"
#include "panic.h"
#include "print.h"
#include "slice.h"
#include "str.h"

#define RSDP_SIGNATURE "RSD PTR "
#define ACPI1_REVISION_NUM  0

struct RSDP* rsdp_addr = NULL;

void walk_acpi_tables(void);

void setup_acpi(void) {
  if (!rsdp_addr ||
      !slceq(TO_SLICE(rsdp_addr->signature), AS_SLICE(RSDP_SIGNATURE)))
    panic("no ACPI RSDP found");

  uint8_t* rsdp_bytes = (uint8_t*)rsdp_addr;
  uint8_t sum = 0;
  for (size_t i = 0; i < 20; i++) sum += rsdp_bytes[i];
  if (sum != 0) panic("incorrect RSDP checksum");

  if (rsdp_addr->revision != ACPI1_REVISION_NUM)
    panic("only ACPI revision 1 supported");

  walk_acpi_tables();
}

struct systable_header {
  uint32_t signature;
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  uint64_t oem_id : 48;
  uint64_t oem_table_id;
  uint32_t oem_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} __attribute__((packed));

struct rsdt {
  struct systable_header header;
  struct systable_header* entries[];
} __attribute__((packed));

struct madt_controller_header {
  uint8_t type;
  uint8_t length;
} __attribute__((packed));

struct madt_ioapic {
  struct madt_controller_header header;
  uint8_t id;
  uint8_t reserved;
  uint32_t ioapic_addr;
  uint32_t gsi_addr;
} __attribute__((packed));

struct madt {
  struct systable_header header;
  uintptr_t lci_addr;
  uint32_t flags;
  struct madt_controller_header controllers;
} __attribute__((packed));

#define ACPI_SIGN_RSDT (uint32_t)('R' + ('S' << 8) + ('D' << 16) + ('T' << 24))
#define ACPI_SIGN_MADT (uint32_t)('A' + ('P' << 8) + ('I' << 16) + ('C' << 24))

#define IO_APIC_TYPE 1

void walk_acpi_tables(void) {
  struct rsdt* rsdt = (struct rsdt*)rsdp_addr->rsdt_addr;

  if (rsdt->header.signature != ACPI_SIGN_RSDT) panic("invalid rsdt signature");

  size_t entry = (rsdt->header.length - sizeof(struct systable_header)) / 4;

  while (entry--) {
    switch (rsdt->entries[entry]->signature) {
      case ACPI_SIGN_MADT:;
        struct madt* madt = (struct madt*)rsdt->entries[entry];
        struct madt_controller_header* cursor = &madt->controllers;
        while (((uintptr_t)cursor - (uintptr_t)madt) < madt->header.length) {
          switch (cursor->type) {
            case IO_APIC_TYPE:;
              struct madt_ioapic* ioapic = (struct madt_ioapic*)cursor;
              ioapic_addr = (struct ioapic*)ioapic->ioapic_addr;
              break;
          }
          cursor = ((void*)cursor + cursor->length);
        }
        break;
    }
  }
};
