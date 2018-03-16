/*
//
// Created by lolblat on 26/01/18.
//

#ifndef OROS_KEYBOARD_H
#define OROS_KEYBOARD_H

#include "../Util/Util.h"
#include "../cpu/isr.h"
#include "screen.h"


*/
/*
 * TODO:
 * implement the keyboard later.
 * more important is the filesystem.
 *//*

extern "C"
{
    namespace drivers
    {
        enum KeyType
        {
            Char,
            Function,
            Notepad
        };

        struct Key
        {
            KeyType m_type;
            char m_char[8];
        };
        // define basic keyboard.
        class Keyboard
        {
            private:
                bool m_numlock;

                static Keyboard* m_instance;
                Keyboard();
                char GetKeyFromCode(u8 code);
            public:
                Keyboard* GetInstance();
                void keyboard_init();
        };
    }

    static void keyboard_callback(interrupt_frame frame);
};
#endif //OROS_KEYBOARD_H
*/
