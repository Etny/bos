#ifndef __SLICE_H
#define __SLICE_H

#include <stddef.h>
#include <stdint.h>

#include "alloc.h"

#define __SLICE_Y(type, name, base, length) \
    __SLICE_X(type, name, base, length) \
    __SLICE_X(type*, name ## _ptr, base, length)

#define __SLICE_TYPES(base, length) \
    __SLICE_Y(char,     char,   base,   length) \
    __SLICE_Y(uint8_t,  uint8,  base,   length) \
    __SLICE_Y(uint16_t, uint16, base,   length) \
    __SLICE_Y(uint32_t, uint32, base,   length) \
    __SLICE_Y(uint64_t, uint64, base,   length) \
    __SLICE_Y(int8_t,   int8,   base,   length) \
    __SLICE_Y(int16_t,  int16,  base,   length) \
    __SLICE_Y(int32_t,  int32,  base,   length) \
    __SLICE_Y(int64_t,  int64,  base,   length) \
    __SLICE_Y(void,     void,   base,   length)

#define __SLICE_X(type, name, base, length) \
    struct slice_##name { type* ptr; size_t len; };
__SLICE_TYPES(a, b)
#undef __SLICE_X

#define __SLICE_X(type, name, base, length) \
    type*: (struct slice_##name) { .ptr = (type*)base, .len = length },

#define SLICE(base, length) _Generic((base), \
        __SLICE_TYPES(base, length) \
        default: (struct slice_void_ptr) {.ptr = (void*)base, .len = length} \
)

#define TO_SLICE(base) SLICE(base, (sizeof(base) / sizeof(base[0])))

#define SLICE_STACK(req_type, length) _Generic((req_type*){0}, \
        __SLICE_TYPES(((req_type[length]){0}), length) \
        default: (struct slice_void_ptr) {.ptr = (void*)0, .len = length} \
)

#define SLICE_ALLOC(req_type, length) _Generic((req_type*){0}, \
        __SLICE_TYPES(malloc(sizeof(req_type) * length), length) \
        default: (struct slice_void_ptr) {.ptr = (void*)0, .len = length} \
)

#endif
