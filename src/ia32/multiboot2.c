
#include "multiboot2.h"

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "boot.h"
#include "print.h"

struct __attribute__((aligned(8))) multiboot2_header_tag {
  uint16_t type;
  uint16_t flags;
  uint32_t size;
} __attribute__((packed));

struct multiboot2_header {
  uint32_t magic;
  uint32_t arch;
  uint32_t header_size;
  uint32_t checksum;
  struct multiboot2_header_tag tags[1];
} __attribute__((packed));

#define MB2_HEADER_MAGIC        (uint32_t)0xE85250D6
#define MB2_HEADER_IA32_ID      (uint32_t)0
#define MB2_HEADER_SIZE         (uint32_t)(sizeof(struct multiboot2_header))
#define MB2_HEADER_CHECKSUM     (uint32_t)1 + ~(MB2_HEADER_MAGIC + MB2_HEADER_IA32_ID + MB2_HEADER_SIZE)

struct multiboot2_header mb2_header __attribute__((section(".multiboot"))) = {
    .magic = MB2_HEADER_MAGIC,
    .arch = MB2_HEADER_IA32_ID,
    .header_size = MB2_HEADER_SIZE,
    .checksum = MB2_HEADER_CHECKSUM,
    .tags = {
        {.type = 0, .flags = 0, .size = 8},
    }};

#define MB2_BOOTLOADER_MAGIC 0x36d76289

bool verify_bootloader_signature(uint32_t magic) {
  return magic == MB2_BOOTLOADER_MAGIC;
}

struct mb2_info_tag {
  uint32_t type;
  uint32_t size;
} __attribute__((packed));

struct elf_shdr {
  uint32_t sh_name;
  uint32_t sh_type;
  uint32_t sh_flags;
  void* sh_addr;
  void* sh_offset;
  uint32_t sh_size;
  uint32_t sh_link;
  uint32_t sh_info;
  uint32_t sh_addralign;
  uint32_t sh_entsize;
} __attribute__((packed));

struct mb2_elf_symbols_tag {
  uint32_t type;
  uint32_t size;
  uint32_t num;
  uint32_t entsize;
  uint32_t shndx;
  // uint16_t _reserved;
  struct elf_shdr headers[1];
} __attribute__((packed));

#define MB2_INFO_TYPE_CMDARGS       1
#define MB2_INFO_TYPE_BLNAME        2
#define MB2_INFO_TYPE_ELFSYMBOLS    9

#define ELF_SHT_SYMTAB  2

struct elf_symbols loaded_symbols = {0};
struct bootloader_info bootloader_info_load(void* info_ptr) {
  struct mb2_info_tag* tag = (info_ptr + 8);

  struct bootloader_info info = {0};
  struct mb2_elf_symbols_tag* symbols;
  while (tag->type) {
    switch (tag->type) {
      case MB2_INFO_TYPE_CMDARGS:
        info.args = ((char*)tag) + 8;
        break;

      case MB2_INFO_TYPE_BLNAME:
        info.bootloader_name = ((char*)tag) + 8;
        break;

      case MB2_INFO_TYPE_ELFSYMBOLS:
        symbols = (struct mb2_elf_symbols_tag*)tag;

        for (size_t i = 0; i < symbols->num; i++) {
          if (symbols->headers[i].sh_type != ELF_SHT_SYMTAB) continue;
          loaded_symbols.symtab =
              (struct elf_symtab*)symbols->headers[i].sh_addr;
          loaded_symbols.strtab =
              symbols->headers[symbols->headers[i].sh_link].sh_addr;
          loaded_symbols.entries =
              symbols->headers[i].sh_size / symbols->headers[i].sh_entsize;
        }
        break;
    }

    // Move to the next tag, accounting for the 8-byte alignment
    tag = (struct mb2_info_tag*)(((intptr_t)tag + tag->size + 7) & ~7);
  }

  return info;
}
