#ifndef __PANIC_H
#define __PANIC_H

#include <stdint.h>
void _panic(const char* error, const char* file, const char* line)
    __attribute__((noreturn));

#define __STRINGIFY(x) __STRINGIFY2(x)
#define __STRINGIFY2(x) #x
#define __LINE_STRING __STRINGIFY(__LINE__)

#define panic(error) _panic(error, __FILE__, __LINE_STRING)

#define REQUIRE(cond)                       \
do {                                        \
    if (!(cond))                            \
        panic("Requirement failed: "#cond); \
} while(0);

void print_stack(void);

#endif  // !__PANIC_H
