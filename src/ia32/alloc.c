
#include "alloc.h"

#include <stddef.h>
#include <stdint.h>

#include "panic.h"
#include "print.h"

#define HEAP_SIZE (ptrdiff_t)0x0FFFFFFF

extern char _end;

char* head = &_end;

void* malloc(size_t size) {
  char* ret = head;
  head += size;

  // For alignment
  head = (char*)(((intptr_t)head + 7) & ~7);
  REQUIRE(head - &_end < HEAP_SIZE);

  // 0x0A
  // 0b01010
  // 0b10001
  // 0b10000

  return ret;
}

void free(__attribute__((unused)) void* ptr) {}
