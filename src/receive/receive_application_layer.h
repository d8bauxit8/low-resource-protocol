/* 
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef RECEIVE_APPLICATION_LAYER_H
#define    RECEIVE_APPLICATION_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "receive_structures.h"

typedef unsigned char (*_LRPReceiveFrameController)(_FrameData *const frameData);

void LRP_initReceiveApplicationLayer(_LRPReceiveLayer *const receiveLayer, const unsigned char *const receiveDeviceId,
                                     _LRPFrame *const receiveFrameBuffer,
                                     const unsigned char const receiveFrameBufferLength);

void LRP_receiveApplicationLayerController(_LRPReceiveLayer *const receiveLayer,
                                           _LRPReceiveFrameController *const receiveFrameControllerList,
                                           const unsigned char const receiveFrameControllerListLength);

#ifdef    __cplusplus
}
#endif

#endif    /* RECEIVE_APPLICATION_LAYER_H */

