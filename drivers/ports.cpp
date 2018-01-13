//
// Created by lolblat on 12/01/18.
//
#include "ports.h"

unsigned char port_byte_in(unsigned short port)
{
    unsigned char res;
    __asm__("in %%dx, %%al" : "=a" (res) : "d" (port));
    return res;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__("out %%al, %%dx" : : "a" (data),  "d" (port));
}

unsigned short port_word_in(unsigned short port)
{
    unsigned short res;
    __asm__("in %%dx, %%ax": "=a" (res) : "d" (port));
    return res;
}

void port_word_out(unsigned short port, unsigned short data)
{
    __asm__("out %%ax,%%dx" : : "a" (data), "d" (port));
}