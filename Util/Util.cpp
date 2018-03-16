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
u32 Util::strlen(const char* str )
{
    const char * tmp  = str;
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
void Util::to_string(u8 number, char* buffer)
{

}


/*
 * Util function: to_hex with u8 as param
 * --------------------------------------
 * Convert u8 data type to hex string.
 *
 */



void Util::to_hex(u8 number, char* buffer)
{
    u8 first_half = (number >> 4) & 0xF;
    u8 last_half = number & 0xF;

    buffer[0] = const_alpha_bet_buffer[first_half];
    buffer[1] = const_alpha_bet_buffer[last_half];
}