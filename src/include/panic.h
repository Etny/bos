#ifndef __PANIC_H
#define __PANIC_H

void panic(const char* error) __attribute__((noreturn));

#define __STRINGIFY(x) __STRINGIFY2(x)
#define __STRINGIFY2(x) #x
#define __LINE_STRING __STRINGIFY(__LINE__)
#define __FILE_STRING __STRINGIFY(__FILE__)

#define REQUIRE(cond)                       \
do {                                        \
    if (!(cond))                            \
        panic("requirement failed: "#cond" at " __FILE_STRING":"__LINE_STRING); \
} while(0);

#endif  // !__PANIC_H
