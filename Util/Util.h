//
// Created by lolblat on 24/01/18.
//

#ifndef OROS_UTIL_H
#define OROS_UTIL_H

#include "../cpu/types.h"
#include "../drivers/screen.h"
#include <stdarg.h>
extern "C"
{
    static const char const_alpha_bet_buffer[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    class Util
    {
        public:
            static u32 strlen(const char* str);
            static void to_string(u64 number,char* buffer);
            static void to_hex(u64 number,char* buffer);

            static void memcopy(void* src, void* dst, u32 size);
            static void memset(void* src, u32 size, char data);
            static void memread(void* src, void* dst, u32 size_of_var, u32 n);
            static u32 number_size_u32(u32 nubmer);
            static void printf(const char* str, ...);
            static void PrintLongLong(long_long long_param);
    };
};
#endif //OROS_UTIL_H
