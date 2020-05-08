/*
 * File:   receive_structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef RECEIVE_STRUCTURES_H
#define    RECEIVE_STRUCTURES_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../frame.h"

// Status flow
// 1. In receiving
// 2. Ready to check
// 3. Ready to read
// 4. Ready to redefine
#define RECEIVE_FRAME_IN_RECEIVING 0b00000001
#define RECEIVE_FRAME_READY_TO_CHECK 0b00000010
#define RECEIVE_FRAME_READY_TO_READ 0b00000011

typedef struct {
    _LRPFrame *frameBuffer;
    unsigned char indexOfReadBytes;
    unsigned char linkLayerStatus;
    unsigned char *receiveDeviceId;

    _LRPFrame *handlerCurrentFrame;
    _LRPFrame *controllerCurrentFrame;
} _LRPReceiveLayer;

#ifdef    __cplusplus
}
#endif

#endif    /* RECEIVE_STRUCTURES_H */


