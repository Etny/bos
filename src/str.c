#include "str.h"

#include <stdbool.h>
#include <stdint.h>

#include "panic.h"
#include "slice.h"

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

char *itos(int32_t val, struct slice_char buf) {
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

  REQUIRE(required_len <= buf.len);
  total_len = required_len;

  buf.ptr[required_len--] = 0;

  if (val < 0) {
    buf.ptr[0] = '-';
    val *= -1;
  }

  do {
    char c = (val % 10) + '0';
    buf.ptr[required_len--] = c;
    val = (val - (val % 10)) / 10;
  } while (val > 0);

  return buf.ptr + total_len;
}

static char hex_map[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char *itohex(uint32_t val, struct slice_char buf) {
  REQUIRE(buf.len >= 3);
  strcpy(buf.ptr, "0x");

  size_t len = 0, idx = 2;
  for (; len < 4; len++) {
    REQUIRE(idx < buf.len);
    uint8_t slice = (val >> ((3 - len) * 8)) & 0xFF;
    if (slice == 0 && len < 3) continue;
    buf.ptr[idx++] = hex_map[slice >> 4];
    buf.ptr[idx++] = hex_map[slice & 0x0F];
  }

  buf.ptr[idx] = 0;

  return buf.ptr + idx;
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
  } while (src[-1]);
  return dest - 1;
}

int strcmp(const char *s1, const char *s2) {
  int val;
  while (*s1 || *s2) {
    val = *(s1++) - *(s2++);
    if (val) return val;
  }
  return 0;
}
inline bool streq(const char *s1, const char *s2) {
  return (strcmp(s1, s2) == 0);
}
