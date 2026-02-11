#include "terminal.h"

#include <stddef.h>
#include <stdint.h>

#include "str.h"

static inline uint8_t vga_entry_color(enum VGA_COLOR fg, enum VGA_COLOR bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t term_row;
size_t term_col;
uint8_t term_color;
#define TERM_BUFFER_SIZE (VGA_HEIGHT * VGA_WIDTH)
char term_buffer[TERM_BUFFER_SIZE];
uint16_t *vga_buffer = (uint16_t *)VGA_MEMORY;

void term_init(void) {
  term_row = 0;
  term_col = 0;
  term_setcolor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      term_buffer[index] = 0;
      vga_buffer[index] = vga_entry(' ', term_color);
    }
  }
}

void term_flush() {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t vga_index = y * VGA_WIDTH + x;
      const size_t buffer_index = ((term_row + y) % VGA_HEIGHT) * VGA_WIDTH + x;
      if (!term_buffer[buffer_index])
        vga_buffer[vga_index] = vga_entry(' ', term_color);
      else
        vga_buffer[vga_index] =
            vga_entry(term_buffer[buffer_index], term_color);
    }
  }
}

void term_setcolor(enum VGA_COLOR fg, enum VGA_COLOR bg) {
  term_color = vga_entry_color(fg, bg);
}

void term_putchar_at(char c, size_t x, size_t y) {
  term_buffer[y * VGA_WIDTH + x] = c;
}

void term_putchar(char c) {
  if (c == '\n') {
    term_nextline();
    return;
  }

  term_putchar_at(c, term_col, term_row);

  if (++term_col == VGA_WIDTH) {
    term_col = 0;
    if (++term_row == VGA_HEIGHT) {
      term_row = 0;
    }
  }
}

void term_nextline(void) {
  do {
    term_putchar(0);
  } while (term_col != 0);
}

void term_writestr(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++) term_putchar(data[i]);
}

void term_writeline(const char *str) {
  term_writestr(str, strlen(str));
  term_writestr("\n", 1);
  term_flush();
}

void term_write(const char *str) {
  term_writestr(str, strlen(str));
  term_flush();
}
