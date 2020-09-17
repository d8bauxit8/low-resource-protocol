/*
 * File:   line_code_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_LINE_CODE_4B5B_H
#define    LRP_LINE_CODE_4B5B_H

#ifdef    __cplusplus
extern "C" {
#endif

// Recommend control codes
// https://en.wikipedia.org/wiki/4B5B
#define LINE_CODE_4B5B_START_DELIMITER_BYTE 0b11000000
#define LINE_CODE_4B5B_END_DELIMITER_BYTE 0b01101000

typedef struct _LRPLineCode4B5B {
    unsigned char index;
    unsigned char *buffer[2];
} _LRPLineCode4B5B;

void LRP_4B5B_reset(_LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(_LRPLineCode4B5B *lineCode4B5B);

void
LRP_4B5B_addEncodedByteToBufferOfEncodedBits(_LRPLineCode4B5B *lineCode4B5B, const unsigned char *data);

unsigned char LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(_LRPLineCode4B5B *lineCode4B5B,
                                                                    unsigned char *decodingState);

unsigned char LRP_4B5B_isDecodingFailed(const unsigned char *decodingState);

unsigned char LRP_4B5B_isBufferOfEncodedBitsReadyToReadAnEncodedByte(_LRPLineCode4B5B *lineCode4B5B);

void LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(_LRPLineCode4B5B *lineCode4B5B,
                                                          const unsigned char *data);

unsigned char LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(_LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_isThereRemainingBitsInBufferOfEncodedBits(_LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_readAnRemainingBitsFromBufferOfEncodedBits(_LRPLineCode4B5B *lineCode4B5B);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_LINE_CODE_4B5B_H */

