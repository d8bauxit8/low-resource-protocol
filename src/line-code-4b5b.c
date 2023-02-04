#include "line-code-4b5b.h"

#define ENCODED_HIGH_BITS_SHIFT 5u
#define DECODED_HIGH_BITS_SHIFT 4u
#define HIGH_CODE_INDEX_SHIFT 4u

#define DECODED_HIGH_BITS_MASK 0b11110000u
#define DECODED_LOW_BITS_MASK 0b00001111u

#define ENCODED_BITS_MASK 0b00011111u
#define ENCODED_TEN_BITS_MASK 0b0000001111111111u

#define NUMBER_OF_BITS_IN_A_DECODED_BYTE 8u
#define NUMBER_OF_BITS_IN_AN_ENCODED_BYTE 10u
#define NUMBER_OF_NON_READ_BITS_IN_AN_ENCODED_BYTE (NUMBER_OF_BITS_IN_AN_ENCODED_BYTE - NUMBER_OF_BITS_IN_A_DECODED_BYTE)
#define NUMBER_OF_REMAINDER_BITS_FROM_16_BITS_AFTER_ENCODE_A_BYTE (16u - NUMBER_OF_BITS_IN_AN_ENCODED_BYTE)

#define FIRST_BUFFER_ITEM 0u
#define SECOND_BUFFER_ITEM 1u

#define NUMBER_OF_4B5B_CODES 16u

// 4B5B codes
// https://en.wikipedia.org/wiki/4B5B
const unsigned char codesOf4B5B[] = {
        0b11110, // 0 (0b0000) - 11110
        0b01001, // 1 (0b0001) - 01001
        0b10100, // 2 (0b0010) - 10100
        0b10101, // 3 (0b0011) - 10101
        0b01010, // 4 (0b0100) - 01010
        0b01011, // 5 (0b0101) - 01011
        0b01110, // 6 (0b0110) - 01110
        0b01111, // 7 (0b0111) - 01111
        0b10010, // 8 (0b1000) - 10010
        0b10011, // 9 (0b1001) - 10011
        0b10110, // 10 (0b1010) - A - 10110
        0b10111, // 11 (0b1011) - B - 10111
        0b11010, // 12 (0b1100) - C - 11010
        0b11011, // 13 (0b1101) - D - 11011
        0b11100, // 14 (0b1110) - E - 11100
        0b11101  // 15 (0b1111) - F - 11101
};

/**
 * Private method definitions
 */
unsigned char LRP_4B5B_decode(const unsigned short *data, LRPLineCode4B5BDecodingState *decodingState);

unsigned short LRP_4B5B_encode(const unsigned char *data);

void LRP_4B5B_rotateBufferOfEncodedBitsPointer(LRPLineCode4B5B *lineCode4B5B);

/**
 * Public method declarations
 */
void LRP_4B5B_reset(LRPLineCode4B5B *const lineCode4B5B) {
    lineCode4B5B->index = 0;
    lineCode4B5B->buffer[0] =
    lineCode4B5B->buffer[1] = 0;
}

unsigned char LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(const LRPLineCode4B5B *const lineCode4B5B) {
    return lineCode4B5B->index >= NUMBER_OF_BITS_IN_AN_ENCODED_BYTE;
}

void
LRP_4B5B_addEncodedByteToBufferOfEncodedBits(LRPLineCode4B5B *const lineCode4B5B, const unsigned char *const data) {
    unsigned char mask = 0xFFu << lineCode4B5B->index;
    lineCode4B5B->buffer[FIRST_BUFFER_ITEM] =
            (unsigned char) ((unsigned char) (*data << lineCode4B5B->index) |
                             (unsigned char) ~mask) &
            (unsigned char) (mask | lineCode4B5B->buffer[FIRST_BUFFER_ITEM]);

    lineCode4B5B->buffer[SECOND_BUFFER_ITEM] =
            (*data >> (unsigned char) (NUMBER_OF_BITS_IN_A_DECODED_BYTE - lineCode4B5B->index));
    // Increase the index by the number of saved bits
    lineCode4B5B->index = lineCode4B5B->index + NUMBER_OF_BITS_IN_A_DECODED_BYTE;
}

unsigned char LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(LRPLineCode4B5B *const lineCode4B5B,
                                                                    LRPLineCode4B5BDecodingState *const decodingState) {
    const unsigned short encodedBits =
            // Set high bits
            (unsigned short) (lineCode4B5B->buffer[SECOND_BUFFER_ITEM] << NUMBER_OF_BITS_IN_A_DECODED_BYTE) |
            // Set low bits
            lineCode4B5B->buffer[FIRST_BUFFER_ITEM];
    // Shift remaining bits
    lineCode4B5B->buffer[SECOND_BUFFER_ITEM] =
            lineCode4B5B->buffer[SECOND_BUFFER_ITEM] >> NUMBER_OF_NON_READ_BITS_IN_AN_ENCODED_BYTE;
    LRP_4B5B_rotateBufferOfEncodedBitsPointer(lineCode4B5B);
    // Decrease the index by the number of read bits
    lineCode4B5B->index = lineCode4B5B->index - NUMBER_OF_BITS_IN_AN_ENCODED_BYTE;
    // Decode the encoded bits
    return LRP_4B5B_decode(&encodedBits, decodingState);
}

unsigned char LRP_4B5B_isDecodingFailed(const LRPLineCode4B5BDecodingState *const decodingState) {
    return *decodingState == LRPLineCode4B5BDecodingState_FailedDecoding;
}

unsigned char LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(const LRPLineCode4B5B *const lineCode4B5B) {
    return lineCode4B5B->index <= NUMBER_OF_REMAINDER_BITS_FROM_16_BITS_AFTER_ENCODE_A_BYTE;
}

void LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(LRPLineCode4B5B *const lineCode4B5B,
                                                          const unsigned char *const data) {
    const unsigned short encodedData = LRP_4B5B_encode(data);
    lineCode4B5B->buffer[FIRST_BUFFER_ITEM] =
            (unsigned char) (encodedData << lineCode4B5B->index) | lineCode4B5B->buffer[FIRST_BUFFER_ITEM];
    lineCode4B5B->buffer[SECOND_BUFFER_ITEM] =
            (unsigned char) (encodedData >> (NUMBER_OF_BITS_IN_A_DECODED_BYTE - lineCode4B5B->index)) |
            lineCode4B5B->buffer[SECOND_BUFFER_ITEM];
    lineCode4B5B->index = lineCode4B5B->index + NUMBER_OF_BITS_IN_AN_ENCODED_BYTE;
}

unsigned char LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(LRPLineCode4B5B *const lineCode4B5B) {
    const unsigned char encodedByte = lineCode4B5B->buffer[FIRST_BUFFER_ITEM];

    LRP_4B5B_rotateBufferOfEncodedBitsPointer(lineCode4B5B);
    lineCode4B5B->index = lineCode4B5B->index - NUMBER_OF_BITS_IN_A_DECODED_BYTE;
    return encodedByte;
}

unsigned char LRP_4B5B_haveRemainingBitsInBufferOfEncodedBits(const LRPLineCode4B5B *const lineCode4B5B) {
    return lineCode4B5B->index != 0;
}

unsigned char LRP_4B5B_readARemainingBitsFromBufferOfEncodedBits(LRPLineCode4B5B *const lineCode4B5B) {
    lineCode4B5B->index = 0;
    return lineCode4B5B->buffer[FIRST_BUFFER_ITEM];
}

/**
 * Private method declarations
 */
unsigned char LRP_4B5B_decode(const unsigned short *const data, LRPLineCode4B5BDecodingState *const decodingState) {
    unsigned char decodedByte = 0;
    unsigned char encodedLowBits = *data & ENCODED_BITS_MASK;
    unsigned char encodedHighBits = (unsigned char) (*data >> ENCODED_HIGH_BITS_SHIFT) & ENCODED_BITS_MASK;
    *decodingState = LRPLineCode4B5BDecodingState_FailedDecoding;

    unsigned char i = 0;
    for (; i < NUMBER_OF_4B5B_CODES; i++) {
        if (codesOf4B5B[i] == encodedLowBits) {
            decodedByte = decodedByte | i;
            encodedLowBits = 0;
        }

        if (codesOf4B5B[i] == encodedHighBits) {
            decodedByte = (unsigned char) (i << DECODED_HIGH_BITS_SHIFT) | decodedByte;
            encodedHighBits = 0;
        }

        if (!encodedLowBits && !encodedHighBits) {
            *decodingState = LRPLineCode4B5BDecodingState_SuccessfulDecoding;
            break;
        }
    }
    return decodedByte;
}

unsigned short LRP_4B5B_encode(const unsigned char *const data) {
    return ENCODED_TEN_BITS_MASK &
           (unsigned short) ((unsigned short) (
                   codesOf4B5B[((*data & DECODED_HIGH_BITS_MASK)
                           >> HIGH_CODE_INDEX_SHIFT)]
                           << ENCODED_HIGH_BITS_SHIFT) |
                             codesOf4B5B[(*data & DECODED_LOW_BITS_MASK)]);
}

void LRP_4B5B_rotateBufferOfEncodedBitsPointer(LRPLineCode4B5B *const lineCode4B5B) {
    lineCode4B5B->buffer[FIRST_BUFFER_ITEM] = lineCode4B5B->buffer[SECOND_BUFFER_ITEM];
    lineCode4B5B->buffer[SECOND_BUFFER_ITEM] = 0u;
}