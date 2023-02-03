unsigned char LRP_ParityBit_createOddParityBit(unsigned char data) {
    unsigned char parity = 1;

    while (data) {
        parity ^= data & 1u;
        data >>= 1u;
    }
    return parity;
}

unsigned char LRP_ParityBit_isInvalidOddParityBit(unsigned char data, const unsigned char *const parityBit) {
    data = data ^ (unsigned char) (data >> 4u);
    data = data ^ (unsigned char) (data >> 2u);
    data = data ^ (unsigned char) (data >> 1u);
    return (data & 1u) == *parityBit;
}