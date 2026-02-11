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

void itos(int32_t val, char *buffer, size_t bufsize) {
  int32_t temp = val;
  size_t required_len = 1;

  if (temp < 0) {
    required_len++;
    temp *= -1;
  }
  while (temp >= 10) {
    required_len++;
    temp = (temp - (temp % 10)) / 10;
  }

  REQUIRE(required_len <= bufsize);

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
}
