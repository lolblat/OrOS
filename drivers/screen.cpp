//
// Created by lolblat on 12/01/18.
//
#include "screen.h"
unsigned int strlen(const char * c); //TODO: when implementing libc, move that function.

void terminal_clear() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            *(unsigned short *) ((unsigned short*)VGA_ADDRESS + y * SCREEN_HEIGHT + x) = GetVGACode(' ');
        }
    }
    index_x = 0;
    index_y = 0;
}

void terminal_write_char(const char c)
{
    unsigned short code = GetVGACode(c);
    *((unsigned  short*)VGA_ADDRESS + index_y * SCREEN_HEIGHT + index_x) = code;
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

void terminal_change_colors(vga_colors arg_fore_color, vga_colors arg_back_color)
{
    default_fore_color = arg_fore_color;
    default_back_color = arg_back_color;
}

void terminal_write_string(const char* str)
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