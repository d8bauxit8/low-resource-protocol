/*
 * File:   parity_bit.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_PARITY_BIT_H
#define    LRP_PARITY_BIT_H

#ifdef    __cplusplus
extern "C" {
#endif

unsigned char LRP_ParityBit_createOddParityBit(unsigned char data);

unsigned char LRP_ParityBit_isInvalidOddParityBit(unsigned char data, const unsigned char *const parityBit);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_PARITY_BIT_H */

