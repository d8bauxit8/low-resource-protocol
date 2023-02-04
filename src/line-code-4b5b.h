/*
 * File:   line-code-4b5b.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_H
#define LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "data/line-code-4b5b.data.h"

void LRP_4B5B_reset(LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(const LRPLineCode4B5B *lineCode4B5B);

void
LRP_4B5B_addEncodedByteToBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B, const unsigned char *data);

unsigned char LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B,
                                                                    unsigned char *decodingState);

unsigned char LRP_4B5B_isDecodingFailed(const unsigned char *decodingState);

unsigned char LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(const LRPLineCode4B5B *lineCode4B5B);

void LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B,
                                                          const unsigned char *data);

unsigned char LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_haveRemainingBitsInBufferOfEncodedBits(const LRPLineCode4B5B *lineCode4B5B);

unsigned char LRP_4B5B_readARemainingBitsFromBufferOfEncodedBits(LRPLineCode4B5B *lineCode4B5B);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_H */

