/*
 * File:   line-code-4b5b.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_H
#define    LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_H

#ifdef    __cplusplus
extern "C" {
#endif

// Recommend control codes
// https://en.wikipedia.org/wiki/4B5B
#define LINE_CODE_4B5B_START_DELIMITER_BYTE 0b11000000
#define LINE_CODE_4B5B_STOP_DELIMITER_BYTE 0b01101000

typedef struct LRPLineCode4B5B {
    unsigned char index;
    unsigned char buffer[2];
} LRPLineCode4B5B;

void LRP_4B5B_reset(LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(LRPLineCode4B5B *lineCode4B5B);

void
LRP_4B5B_addEncodedByteToBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B, const unsigned char *data);

unsigned char LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B,
                                                                    unsigned char *decodingState);

unsigned char LRP_4B5B_isDecodingFailed(const unsigned char *decodingState);

unsigned char LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(LRPLineCode4B5B *lineCode4B5B);

void LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B,
                                                          const unsigned char *data);

unsigned char LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_isThereRemainingBitsInBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_readAnRemainingBitsFromBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_H */

