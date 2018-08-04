//
// Created by lolblat on 24/01/18.
//

#include "Util.h"


/*
 * Util function: strlen
 * ---------------------
 * Count the chars in a given str.
 *
 */
u32 Util::strlen(const u8* str )
{
    const u8 * tmp  = str;
    u32 sum = 0;
    while(*tmp)
    {
        sum++;
        tmp++;
    }
    return sum;
}

/*
 * Util function: to_string with u8 as param
 * -----------------------------------------
 * Convert u8 data type to string. and put it in a buffer
 *
 */
void Util::to_string(u64 number, char* buffer)
{
    u32 index = number_size_u32(number) - 1;
    buffer[number_size_u32(number)] = 0;
    if(number == 0)
    {
        buffer[index] = '0';
    }
    while(number != 0)
    {
        int tmp = number % 10;
        buffer[index] = tmp + '0';
        number /= 10;
        index--;
    }

}


/*
 * Util function: to_hex with u8 as param
 * --------------------------------------
 * Convert u8 data type to hex string.
 *
 */



void Util::to_hex(u64 number, char* buffer)
{
    /*
    u8 first_half = (number >> 4) & 0xF;
    u8 last_half = number & 0xF;

    buffer[0] = const_alpha_bet_buffer[first_half];
    buffer[1] = const_alpha_bet_buffer[last_half];
*/
    buffer[0] = '0';
    buffer[1] = 'x';
    u32 index = 14;
    while(number != 0)
    {
        u8 part = number & 0xf;
        buffer[index + 2] = const_alpha_bet_buffer[part];
        index--;
        number >>= 4;
    }
    for(u32 i = 0; i  <= index; i++)
    {
        buffer[i + 2] = '0';
    }

}

u32 Util::strcmp(const u8 *str1, const u8 *str2)
{
    if(strlen(str1) > strlen(str2))
    {
        return 1;
    }
    else if(strlen(str2) > strlen(str1))
    {
        return -1;
    }
    else
    {
        for(u32 i = 0; i < strlen(str1); i++)
        {
            if(*str1 > *str2)
            {
                return 1;
            }
            else if(*str2 > *str1)
            {
                return -1;
            }
        }
    }
    return 0;
}
const u8* Util::strstr(const u8 *str1, const u8 *str2)
{
    const u8* ptr = str1;
    const u8* ptr2 = str2;
    while(*ptr != '\x00')
    {
        if(!(*ptr2)) // end of str2, need to return.
        {
            return (ptr - strlen((const u8*)str2));
        }
        if(*ptr == *ptr2)
        {
            ptr2++;
        }
        else
        {
            ptr2 = str2;
        }
        ptr++;
    }
    return (const u8*)0;
}

/*
 * Util function: return the number length of int
 * ----------------------------------------------
 * Get number size
 */
u32 Util::number_size_u32(u32 number)
{
    u32 count = 0;
    if(number == 0)
    {
        return 1;
    }

    while(number != 0)
    {
        count += 1;
        number /= 10;
    }
    return count;
}

/*
 * Util function: copy memory from one buffer to another.
 */
void Util::memcopy(void *src, void *dst, u32 size)
{
    for(u32 i = 0; i < size; i++)
    {
        *(char*)((char*)dst + i) = *(char*)((char*)src + i);
    }
}

/*
 * Util function: set memory to some value
 */
void Util::memset(void *src, u32 size, char data)
{
    for(u32 i = 0; i < size; i ++)
    {
        *(char*)((char*)src + i) = data;
    }
}

/*
 * Util function: read from memory
 */
void Util::memread(void* src, void* dst, u32 size,u32 n)
{
    for(u32 i = 0; i < n; i++)
    {
        memcopy((void*)(((char*)src) + i * size),(void*)(((char*)dst) + i * size),size);
    }

}

/*
 * Util function: printf for printing, c like
 */
void Util::printf(const char *str, ...)
{
    drivers::Screen s;
    va_list args;
    va_start(args,str);
    bool set = false;
    while(*str != 0)
    {
        if(*str == '%')
        {
            set = true;
        } else if (set) {
            /* why no switch bro */
            if (*str == 'd') {
                u32 num = va_arg(args, u32);
                char str[number_size_u32(num) + 1];
                to_string((u64) num, str);
                s.terminal_write_string(str);
            } else if (*str == 'x') {
                u32 num = va_arg(args, u32);
                char str[18];
                str[17] = 0;
                to_hex(num, str);
                s.terminal_write_string(str);
            } else if (*str == 's')
            {
                char* st = va_arg(args,char*);
                s.terminal_write_string(st);
            }

            set = false;
        }
        else
        {
            s.terminal_write_char(*str);
        }

        str++;
    }
    va_end(args);
}


u32 Util::strsplit(u8 *str1, u8 det)
{
    u32 counter = 0;
    while(*str1)
    {
        if(*str1 == det)
        {
            *str1 = '\x00';
            counter++;
        }
        str1++;
    }
    return counter;
}

u32 Util::strcharcount(const u8* src, const char delm)
{
  u32 counter = 0;
  while(*src)
  {
    if(*src == delm)
    {
      counter++;
    }
    src++;
  }
  return counter;
}

void Util::strpos( u8* str, const char dem)
{
  while(*str)
  {
    if(*str == dem)
    {
      *str = '\x00';
      return;
    }
    str++;
  }
}
