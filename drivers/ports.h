//
// Created by lolblat on 12/01/18.
//

#ifndef OROS_PORTS_H
#define OROS_PORTS_H

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

unsigned short port_word_int(unsigned short port);
void port_word_out(unsigned short port, unsigned short data);

#endif //OROS_PORTS_H
