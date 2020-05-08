/*
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef PARITY_BIT_H
#define    PARITY_BIT_H

#ifdef    __cplusplus
extern "C" {
#endif

unsigned char LRP_createOddParityBit(unsigned char data);

unsigned char LRP_isInvalidOddParityBit(unsigned char data, const unsigned char *const parityBit);

#ifdef    __cplusplus
}
#endif

#endif    /* PARITY_BIT_H */

