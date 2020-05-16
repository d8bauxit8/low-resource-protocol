#include "parity_bit.h"

unsigned char LRP_ParityBit_createOddParityBit(unsigned char data) {
    unsigned char parity = 1;

    while (data) {
        parity ^= data & 1;
        data >>= 1;
    }
    return parity;
}

unsigned char LRP_ParityBit_isInvalidOddParityBit(unsigned char data, const unsigned char *const parityBit) {
    data = data ^ data >> 4;
    data = data ^ data >> 2;
    data = data ^ data >> 1;
    return (data & 1) == *parityBit;
}