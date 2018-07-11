//
// Created by lolblat on 12/01/18.
//

#ifndef OROS_PORTS_H
#define OROS_PORTS_H
#include "../cpu/types.h"
extern "C"
{

    namespace drivers
    {
        class Ports
        {
        private:
            u32 m_port_number;
        public:
            Ports(u32 port_number);
            Ports();
            u8 port_byte_in();
            void port_byte_out(u8 data);

            u16 port_word_in();
            void port_word_out(u16 data);

            void SetPort(u32 port_number);
        };
    }
};

#endif //OROS_PORTS_H
