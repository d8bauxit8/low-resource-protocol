/*
 * File:   parity-bit.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_PARITY_BIT_H
#define    LOW_RESOURCE_PROTOCOL_PARITY_BIT_H

#ifdef    __cplusplus
extern "C" {
#endif

unsigned char LRP_ParityBit_createOddParityBit(unsigned char data);

unsigned char LRP_ParityBit_isInvalidOddParityBit(unsigned char data, const unsigned char *parityBit);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_PARITY_BIT_H */

