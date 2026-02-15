#ifndef __PANIC_H
#define __PANIC_H

#include <stdint.h>

struct panic_context {
  uint32_t ip;
  uint32_t bp;
};

extern void _panic(const char* error, const char* file, const char* line,
                   struct panic_context ctx) __attribute__((noreturn));

#define __STRINGIFY(x) __STRINGIFY2(x)
#define __STRINGIFY2(x) #x
#define __LINE_STRING __STRINGIFY(__LINE__)

#define panic(error) _panic(error, __FILE__, __LINE_STRING, (struct panic_context){0})
#define panic_from(error, instptr, baseptr) _panic(error, __FILE__, __LINE_STRING, (struct panic_context){.ip = instptr, .bp = baseptr})

#define REQUIRE(cond)                       \
do {                                        \
    if (!(cond))                            \
        panic("Requirement failed: "#cond); \
} while(0);

void print_stack(void);
void print_stack_from(uint32_t ip, uint32_t bp);

#endif  // !__PANIC_H
