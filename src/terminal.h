#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

enum VGA_COLOR {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

void term_init(void);
void term_setcolor(enum VGA_COLOR fg, enum VGA_COLOR bg);
void term_printchar(char c);
void term_putchar_at(char c, size_t x, size_t y);
void term_write(const char *str);
void term_writeline(const char *str);

#endif // !TERMINAL_H
