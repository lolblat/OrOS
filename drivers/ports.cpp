//
// Created by lolblat on 12/01/18.
//
#include "ports.h"
using namespace drivers;
Ports::Ports(u32 port_number)
{
    this->m_port_number = port_number;
}
Ports::Ports()
{
    m_port_number = 0;
}
u8 Ports::port_byte_in()
{
    u8 res;
    __asm__("in %%dx, %%al" : "=a" (res) : "d" (m_port_number));
    return res;
}

void Ports::port_byte_out(u8 data)
{
    __asm__("out %%al, %%dx" : : "a" (data),  "d" (m_port_number));
}

u16 Ports::port_word_in()
{
    u16 res;
    __asm__("in %%dx, %%ax": "=a" (res) : "d" (m_port_number));
    return res;
}

void Ports::port_word_out(u16 data)
{
    __asm__("out %%ax,%%dx" : : "a" (data), "d" (m_port_number));
}
void Ports::SetPort(u32 port_number)
{
    m_port_number = port_number;
}