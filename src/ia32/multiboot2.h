#ifndef __MULTIBOOT2_H
#define __MULTIBOOT2_H

#include <stdint.h>
struct elf_symtab {
  uint32_t st_name;
  uint32_t st_value;
  uint32_t st_size;
  uint8_t st_info;
  uint8_t st_other;
  uint16_t st_shndx;
} __attribute__((packed));

struct elf_symbols {
  struct elf_symtab* symtab;
  uint32_t entries;
  const char* strtab;
};

#define ELF32_ST_TYPE(i) (((i)&0xf)
#define ELF32_TYPE_FUNC       2
#define ELF32_TYPE_LABEL       0

extern struct elf_symbols loaded_symbols;
#endif
