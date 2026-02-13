#include "str.h"

#include <stdint.h>

#include "panic.h"

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) len++;
  return len;
}

int32_t atoi(const char *str) {
  int32_t out = 0;
  int32_t sign = 1;

  while (*str == '-' || *str == '+') {
    if (*str == '-') sign *= -1;
    str++;
  }

  while (*str >= '0' && *str <= '9') {
    out = (out * 10) + (*str - '0');
    str++;
  }

  return sign * out;
}

char *itos(int32_t val, char *buffer, size_t bufsize) {
  int32_t temp = val;
  size_t total_len, required_len = 1;

  if (temp < 0) {
    required_len++;
    temp *= -1;
  }
  while (temp >= 10) {
    required_len++;
    temp = (temp - (temp % 10)) / 10;
  }

  REQUIRE(required_len <= bufsize);
  total_len = required_len;

  buffer[required_len--] = 0;

  if (val < 0) {
    buffer[0] = '-';
    val *= -1;
  }

  do {
    char c = (val % 10) + '0';
    buffer[required_len--] = c;
    val = (val - (val % 10)) / 10;
  } while (val > 0);

  return buffer + total_len;
}

void *memcpy(void *restrict dest, void *const restrict src, size_t len) {
  const uint8_t *src_p = (const uint8_t *)src;
  uint8_t *dest_p = (uint8_t *)dest;
  for (; len; len--) *dest_p++ = *src_p++;

  return dest;
}

void *memmove(void *dest, void *src, size_t len) {
  const uint8_t *src_p = (const uint8_t *)src;
  uint8_t *dest_p = (uint8_t *)dest;
  if (src > dest)
    for (; len; len--) *dest_p++ = *src_p++;
  else
    while (len--) dest_p[len] = src_p[len];
  return dest;
}

char *strcpy(char *restrict dest, const char *restrict src) {
  do {
    *dest++ = *src++;
  } while (*src);
  return dest;
}
