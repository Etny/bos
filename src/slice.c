#include "slice.h"

int slccmp(struct slice_char s1, struct slice_char s2) {
  if (s1.len < s2.len) return s2.ptr[s1.len];
  if (s2.len < s1.len) return -s1.ptr[s2.len];
  size_t off = 0;
  int val;
  while (off < s1.len && off < s2.len) {
    val = s1.ptr[off] - s2.ptr[off];
    if (val) return val;
    off++;
  }

  return 0;
}
bool slceq(struct slice_char s1, struct slice_char s2) {
  if (s1.len != s2.len) return false;
  return (slccmp(s1, s2) == 0);
}
