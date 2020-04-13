/* 
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef RECEIVE_LAYER_H
#define    RECEIVE_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "structures.h"
#include "receive_frame.h"
// Status flow
// 1. In progress
// 2. Completed
// 3. Ready to redefine
#define RECEIVE_FRAME_COMPLETED 0b00000010

#define RECEIVE_LAYER_STATUS_OK 0
#define RECEIVE_LAYER_STATUS_ERROR 1

void LRP_initReceiveLayer(_LRPReceiveLayer *const receiveLayer, const unsigned char *const receiveDeviceId,
                          _LRPFrame *const receiveFrameBuffer,
                          unsigned char frameBufferLength,
                          void (*LRP_framingErrorHandler)(void),
                          void (*LRP_overrunErrorHandler)(void),
                          void (*LRP_parityBitErrorHandler)(void));

void
LRP_receiveLayerController(_LRPReceiveLayer *const receiveLayer, unsigned char data,
                           const unsigned char *const parityBit,
                           const unsigned char *const framingError,
                           const unsigned char *const overrunError);

void LRP_noReceiveErrorCallBack(void);

#ifdef    __cplusplus
}
#endif

#endif    /* RECEIVE_LAYER_H */

