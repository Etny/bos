#ifndef __BOOT_H
#define __BOOT_H

#include <stdbool.h>
#include <stdint.h>

bool verify_bootloader_signature(uint32_t magic);

struct bootloader_info {
  char* args;
  char* bootloader_name;
};

struct bootloader_info bootloader_info_load(void* info_ptr);

#endif  // !__BOOT_H
