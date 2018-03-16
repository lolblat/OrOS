/*
//
// Created by lolblat on 26/01/18.
//

#include "keyboard.h"
using namespace drivers
void keyboard_callback(interrupt_frame frame)
{
    Screen s;
    u8 code = Ports::port_byte_in(0x60);
    char buff[2];
    Util::to_hex(code,buff);
    s.terminal_write_char(buff[0]);
    s.terminal_write_char(buff[1]);
    s.terminal_write_string(" ");
}

void Keyboard::keyboard_init()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
    m_numlock = false;
}

void Keyboard::GetKeyFromCode(u8 code, Key &key)
{
    key.m_type = KeyType::Char;
    switch (code)
    {
        case 0x01:
        {
            key.m_char = "ESC";
            break;
        }
        case 0x02:
        {
            key.m_char = "1";
            break;
        }
        case 0x03:
        {
            key.m_char = "2";
            break;
        }
        case 0x04:
        {
            key.m_char = "3";
            break;
        }
        case 0x05:
        {
            key.m_char = "4";
            break;
        }
        case 0x06:
        {
            key.m_char = "5";
            break;
        }
        case 0x07:
        {
            key.m_char = "6";
            break;
        }
        case 0x08:
        {
            key.m_char = "7";
            break;
        }
        case 0x09:
        {
            key.m_char = "8";
            break;
        }
        case 0x0A:
        {
            key.m_char = "9";
            break;
        }
        case 0x0B:
        {
            key.m_char = "0";
            break;
        }

    }
}*/
