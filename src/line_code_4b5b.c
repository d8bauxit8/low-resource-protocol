#include "line_code_4b5b.h"

#define ENCODED_HIGH_BITS_SHIFT 5
#define DECODED_HIGH_BIT_SHIFT 4

#define DECODED_HIGH_BITS_MASK 0b11110000
#define DECODED_LOW_BITS_MASK 0b00001111

#define ENCODED_BITS_MASK 0b00011111

#define NUMBER_OF_BITS_IN_A_DECODED_BYTE 8
#define NUMBER_OF_BITS_FROM_AN_ENCODED_BYTE 10
#define NUMBER_OF_NON_READ_BITS_FROM_AN_ENCODED_BYTE NUMBER_OF_BITS_FROM_AN_ENCODED_BYTE - NUMBER_OF_BITS_IN_A_DECODED_BYTE

#define FIRST_BUFFER_ITEM 0
#define SECOND_BUFFER_ITEM 1

#define NUMBER_OF_4B5B_CODES 16

// 4B5B codes
const unsigned char const codesOf4B5B[] = {
        0b11110, // 0
        0b01001, // 1
        0b10100, // 2
        0b10101, // 3
        0b01010, // 4
        0b01011, // 5
        0b01110, // 6
        0b01111, // 7
        0b10010, // 8
        0b10011, // 9
        0b10110, // 10 - A
        0b10111, // 11 - B
        0b11010, // 12 - C
        0b11011, // 13 - D
        0b11100, // 14 - E
        0b11101  // 15 - F
};

/**
 * Private method definitions
 */
unsigned char LRP_4B5BDecode(const unsigned short *const data);

unsigned short LRP_4B5BEncode(const unsigned char *const data);

void LRP_4B5BRotateBufferOfEncodedBitsPointer(_LRPLineCode4B5B *const lineCode4B5B);

/**
 * Public method declarations
 */
void LRP_4B5BReset(_LRPLineCode4B5B *const lineCode4B5B) {
    lineCode4B5B->index = 0;
}

unsigned char LRP_4B5BIsBufferOfEncodedBitsReadyToReadADecodedByte(_LRPLineCode4B5B *const lineCode4B5B) {
    return lineCode4B5B->index >= NUMBER_OF_BITS_FROM_AN_ENCODED_BYTE;
}

void
LRP_4B5BAddEncodedByteToBufferOfEncodedBits(_LRPLineCode4B5B *const lineCode4B5B, const unsigned char *const data) {
    unsigned char mask = 0xFF << lineCode4B5B->index;
    *lineCode4B5B->buffer[FIRST_BUFFER_ITEM] =
            // Kifejezés 2. fele:
            // Az adott bitek-et normalizálnom kell, hogy ha egy előző állípotból,
            // az indextől kezdődően felfelé maradt 0-ás bit, azt írjuk felül 1-re.
            // Kifejezés 1. fele:
            // A vett adatokkal töltsük fel az aradék biteket
            ((*data << lineCode4B5B->index) | ~mask) & (mask | *lineCode4B5B->buffer[FIRST_BUFFER_ITEM]);
    // Az előző helyre fltöltött bitek számával toljuk el, s a maradékot írjuk bele a másooik bufferbe
    *lineCode4B5B->buffer[SECOND_BUFFER_ITEM] =
            (*data >> (NUMBER_OF_BITS_IN_A_DECODED_BYTE - lineCode4B5B->index));
    // Növeljük a mentett bitek számával az indexet.
    lineCode4B5B->index = lineCode4B5B->index + NUMBER_OF_BITS_IN_A_DECODED_BYTE;
}

unsigned char LRP_4B5BReadADecodedByteFromBufferOfEncodedBits(_LRPLineCode4B5B *const lineCode4B5B) {
    const unsigned short const encodedBits =
            // Magas bitek beállítása
            (*lineCode4B5B->buffer[SECOND_BUFFER_ITEM] << NUMBER_OF_BITS_IN_A_DECODED_BYTE) |
            // Alacsony bitek beállítása
            *lineCode4B5B->buffer[FIRST_BUFFER_ITEM];
    // Maradék bitek shiftelése
    *lineCode4B5B->buffer[SECOND_BUFFER_ITEM] =
            *lineCode4B5B->buffer[SECOND_BUFFER_ITEM] >> NUMBER_OF_NON_READ_BITS_FROM_AN_ENCODED_BYTE;
    LRP_4B5BRotateBufferOfEncodedBitsPointer(lineCode4B5B);
    // Csökkentsük az olvasott bitek számával az indexet
    lineCode4B5B->index = lineCode4B5B->index - NUMBER_OF_BITS_FROM_AN_ENCODED_BYTE;
    // Dekódoljuk a kódolt bitcsoportot
    return LRP_4B5BDecode(&encodedBits);
}

unsigned char LRP_4B5BIsBufferOfEncodedBitsReadyToReadAnEncodedByte(_LRPLineCode4B5B *const lineCode4B5B) {
    return lineCode4B5B->index < NUMBER_OF_BITS_IN_A_DECODED_BYTE;
}

void LRP_4B5BEncodeDataByteAndAddItToBufferOfEncodedBits(_LRPLineCode4B5B *const lineCode4B5B,
                                                         const unsigned char *const data) {
    const unsigned short const encodedData = LRP_4B5BEncode(data);
    *lineCode4B5B->buffer[FIRST_BUFFER_ITEM] =
            (encodedData << lineCode4B5B->index) | *lineCode4B5B->buffer[FIRST_BUFFER_ITEM];
    *lineCode4B5B->buffer[SECOND_BUFFER_ITEM] =
            (encodedData >> (NUMBER_OF_BITS_IN_A_DECODED_BYTE - lineCode4B5B->index)) |
            *lineCode4B5B->buffer[SECOND_BUFFER_ITEM];
    lineCode4B5B->index = lineCode4B5B->index + NUMBER_OF_BITS_FROM_AN_ENCODED_BYTE;
}

unsigned char LRP_4B5BReadAnEncodedByteFromBufferOfEncodedBits(_LRPLineCode4B5B *const lineCode4B5B) {
    const unsigned char const encodedByte = *lineCode4B5B->buffer[FIRST_BUFFER_ITEM];

    LRP_4B5BRotateBufferOfEncodedBitsPointer(lineCode4B5B);
    lineCode4B5B->index = lineCode4B5B->index - NUMBER_OF_BITS_IN_A_DECODED_BYTE;
    return encodedByte;
}

/**
 * Private method declarations
 */
unsigned char LRP_4B5BDecode(const unsigned short *const data) {
    unsigned char decodedByte = 0;
    unsigned char encodedLowBits = *data & ENCODED_BITS_MASK;
    unsigned char encodedHighBits = (*data >> ENCODED_HIGH_BITS_SHIFT) & ENCODED_BITS_MASK;
    for (unsigned char i = 0; i < NUMBER_OF_4B5B_CODES; i++) {
        if (codesOf4B5B[i] == encodedLowBits) {
            decodedByte = decodedByte | i;
            encodedLowBits = 0;
        }

        if (codesOf4B5B[i] == encodedHighBits) {
            decodedByte = (i << DECODED_HIGH_BIT_SHIFT) | decodedByte;
            encodedHighBits = 0;
        }

        if (!encodedLowBits && !encodedHighBits) {
            break;
        }
    }
    return decodedByte;
}

unsigned short LRP_4B5BEncode(const unsigned char *const data) {
    return (codesOf4B5B[(unsigned char) (*data & DECODED_HIGH_BITS_MASK)] << ENCODED_HIGH_BITS_SHIFT) &
           codesOf4B5B[(unsigned char) (*data & DECODED_LOW_BITS_MASK)];
}

void LRP_4B5BRotateBufferOfEncodedBitsPointer(_LRPLineCode4B5B *const lineCode4B5B) {
    unsigned char *tmp = lineCode4B5B->buffer[FIRST_BUFFER_ITEM];
    lineCode4B5B->buffer[FIRST_BUFFER_ITEM] = lineCode4B5B->buffer[SECOND_BUFFER_ITEM];
    lineCode4B5B->buffer[SECOND_BUFFER_ITEM] = tmp;
}