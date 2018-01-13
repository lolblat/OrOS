//
// Created by lolblat on 12/01/18.
//

#ifndef OROS_SCREEN_H
#define OROS_SCREEN_H

extern "C"
{
enum vga_colors
{
    black = 0x00,
    blue = 0x01,
    green = 0x02,
    cyan = 0x03,
    red = 0x04,
    magenta = 0x05,
    brown = 0x06,
    gray = 0x07,
    dark_gray = 0x08,
    bright_blue = 0x09,
    bright_green = 0x0A,
    bright_cyan = 0x0B,
    bright_red = 0x0C,
    bright_magenta = 0x0D,
    yellow = 0x0E,
    white = 0x0F
};

#define VGA_ADDRESS 0xB8000 //address of vga buffer
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
static vga_colors default_fore_color = white;
static vga_colors default_back_color = black;
static unsigned int index_x = 0;
static unsigned int index_y = 0;

void terminal_clear(); // clear screen
void terminal_write_char(const char c);
void terminal_write_string(const char* str);
void terminal_change_colors(vga_colors fore_color, vga_colors back_color);

unsigned short inline GetVGACode(unsigned char c)
{
    unsigned char color = (default_back_color << 4 | default_fore_color);

    return ((color << 8) | c);
}
};
#endif //OROS_SCREEN_H
