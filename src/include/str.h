#ifndef STR_H
#define STR_H

#include <stddef.h>
#include <stdint.h>

#include "slice.h"

void *memcpy(void *restrict dest, void *restrict src, size_t len);
void *memmove(void *dest, void *src, size_t len);

char *strcpy(char *restrict dest, const char *restrict src);

size_t strlen(const char *str);
int32_t atoi(const char *str);

// char *itos(int32_t val, char *buffer, size_t bufsize);
char *itos(int32_t val, struct slice_char);

#endif  // !STR_H
