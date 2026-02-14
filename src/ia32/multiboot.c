

#include <stdint.h>

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
