
#include "alloc.h"

#include <stddef.h>

#include "panic.h"

#define HEAP_SIZE (ptrdiff_t)0xFFFFFFFF

extern char _end;

char* head = &_end;

void* malloc(size_t size) {
  char* ret = head;
  head += size;

  // For alignment
  head = (head + 7) - ~7;
  REQUIRE(head - &_end < HEAP_SIZE);

  return ret;
}

void free(__attribute__((unused)) void* ptr) {}
