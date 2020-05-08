/*
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LINE_CODE_4B5B_H
#define    LINE_CODE_4B5B_H

#ifdef    __cplusplus
extern "C" {
#endif
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 128 ? '1' : '0'), \
  (byte & 64 ? '1' : '0'), \
  (byte & 32 ? '1' : '0'), \
  (byte & 16 ? '1' : '0'), \
  (byte & 8 ? '1' : '0'), \
  (byte & 4 ? '1' : '0'), \
  (byte & 2 ? '1' : '0'), \
  (byte & 1 ? '1' : '0')

// Recommend control codes
#define START_DELIMITER_BYTE_4B5B 0b00011000
#define END_DELIMITER_BYTE_4B5B 0b00001101

typedef struct _LRPLineCode4B5B {
    unsigned char index;
    unsigned char *buffer[2];
} _LRPLineCode4B5B;

void LRP_4B5BReset(_LRPLineCode4B5B *const lineCode4B5B);

unsigned char LRP_4B5BIsBufferOfEncodedBitsReadyToReadADecodedByte(_LRPLineCode4B5B *const lineCode4B5B);

void LRP_4B5BAddEncodedByteToBufferOfEncodedBits(_LRPLineCode4B5B *const lineCode4B5B, const unsigned char *const data);

unsigned char LRP_4B5BReadADecodedByteFromBufferOfEncodedBits(_LRPLineCode4B5B *const lineCode4B5B);

unsigned char LRP_4B5BIsBufferOfEncodedBitsReadyToReadAnEncodedByte(_LRPLineCode4B5B *const lineCode4B5B);

void LRP_4B5BEncodeDataByteAndAddItToBufferOfEncodedBits(_LRPLineCode4B5B *const lineCode4B5B,
                                                         const unsigned char *const data);

unsigned char LRP_4B5BReadAnEncodedByteFromBufferOfEncodedBits(_LRPLineCode4B5B *const lineCode4B5B);

#ifdef    __cplusplus
}
#endif

#endif    /* LINE_CODE_4B5B_H */

