#ifndef __PRINT_H
#define __PRINT_H

#include <stdint.h>

enum _print_kind { INT, STR };
struct _print_entry {
  enum _print_kind kind;
  union {
    int32_t data_int;
    char* data_str;
  } data;
};

#define __PRINT_ENTRY_INT(num) (struct _print_entry){\
    .kind = INT, \
    .data = {.data_int = (int32_t)num } \
}

#define __PRINT_ENTRY_STR(ptr) (struct _print_entry){\
    .kind = STR, \
    .data = {.data_str = (char*)ptr } \
}

#define __PRINT_ENTRY(data)  _Generic((data), \
int8_t: __PRINT_ENTRY_INT(data), \
int16_t: __PRINT_ENTRY_INT(data), \
int32_t: __PRINT_ENTRY_INT(data), \
int64_t: __PRINT_ENTRY_INT(data), \
uint8_t: __PRINT_ENTRY_INT(data), \
uint16_t: __PRINT_ENTRY_INT(data), \
uint32_t: __PRINT_ENTRY_INT(data), \
uint64_t: __PRINT_ENTRY_INT(data), \
char*:  __PRINT_ENTRY_STR(data)\
        )

void __print(struct _print_entry);

#define print(data) __print(__PRINT_ENTRY(data))

#endif
