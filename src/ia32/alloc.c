
#include "alloc.h"

#include <stddef.h>

#include "panic.h"

#define HEAP_SIZE 0xFFFFFF

char heap[HEAP_SIZE];
size_t head = 0;

void* malloc(size_t size) {
  char* ret = heap + head;
  head += size;

  // For alignment
  head = (head + 7) - ~7;
  REQUIRE(head < HEAP_SIZE);

  return ret;
}

void free(__attribute__((unused)) void* ptr) {}
