//
// Created by lolblat on 24/01/18.
//

#ifndef OROS_UTIL_H
#define OROS_UTIL_H

#include "../cpu/types.h"
extern "C"
{
    static const char const_alpha_bet_buffer[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    class Util
    {
        public:
            static u32 strlen(const char* str);
            static void to_string(u8 number,char* buffer);
            static void to_hex(u8 number,char* buffer);
    };
};
#endif //OROS_UTIL_H
