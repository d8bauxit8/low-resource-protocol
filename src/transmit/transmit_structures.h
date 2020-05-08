/* 
 * File:   transmit_structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef TRANSMIT_STRUCTURES_H
#define    TRANSMIT_STRUCTURES_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../frame.h"

typedef struct {
    _LRPFrame *frameBuffer;
    unsigned char numberOfSentBytes;
    unsigned char status;
    unsigned char *transmitDeviceId;

    _LRPFrame *handlerCurrentFrame;
    _LRPFrame *controllerCurrentFrame;
} _LRPTransmitLayer;

#ifdef    __cplusplus
}
#endif

#endif    /* TRANSMIT_STRUCTURES_H */


