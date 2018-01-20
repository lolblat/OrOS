//
// Created by lolblat on 12/01/18.
//
#include "screen.h"
using namespace drivers;
Screen::vga_colors  Screen::default_fore_color = white;
Screen::vga_colors  Screen::default_back_color = black;
unsigned int Screen::index_x = 0;
unsigned int Screen::index_y = 0;

unsigned int strlen(const char * c); //TODO: when implementing libc, move that function.

void Screen::terminal_clear() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            *(unsigned short *) ((unsigned short*)VGA_ADDRESS + y * SCREEN_WIDTH + x) = GetVGACode(' ');
        }
    }
    index_x = 0;
    index_y = 0;
}

void Screen::terminal_write_char(const char c)
{
    if(c == '\n')
    {
        index_y++;
        index_x = 0;
        return;
    }

    unsigned short code = GetVGACode(c);
    *((unsigned  short*)VGA_ADDRESS + index_y * SCREEN_WIDTH + index_x) = code;
    index_x++;
    if(index_x == SCREEN_WIDTH)
    {
        index_y++;
        index_x = 0;
    }
    if(index_y == SCREEN_HEIGHT)
    {
        //TODO: implement scroll.
    }

}

void Screen::terminal_change_colors(vga_colors arg_fore_color, vga_colors arg_back_color)
{
    default_fore_color = arg_fore_color;
    default_back_color = arg_back_color;
}

void Screen::terminal_write_string(const char* str)
{
    for(unsigned int i = 0; i < strlen(str); i++)
    {
        terminal_write_char(str[i]);
    }
}

unsigned int strlen(const char* str)
{
    unsigned int len = 0;
    while (*str)
    {
        len++;
        str++;
    }
    return len;
}