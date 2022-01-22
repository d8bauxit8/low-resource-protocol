/* 
 * File:   receive-application-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_RECEIVE_APPLICATION_LAYER_H
#define    LOW_RESOURCE_PROTOCOL_RECEIVE_APPLICATION_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "receive-structures.h"

typedef unsigned char (*LRPReceiveFrameController)(LRPFrameData *const frameData);

void LRP_ReceiveApplicationLayer_handler(LRPReceiveSessionProvider *sessionProvider,
                                         LRPReceiveFrameController *receiveFrameControllerList,
                                         unsigned char numberOfReceiveFrameControllers);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_RECEIVE_APPLICATION_LAYER_H */

