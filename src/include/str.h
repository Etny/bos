#ifndef STR_H
#define STR_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *str);
int32_t atoi(const char *str);

void itos(int32_t val, char *buffer, size_t bufsize);
// int64_t atol(const char *str);

#endif  // !STR_H
