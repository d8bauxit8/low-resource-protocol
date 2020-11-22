/* 
 * File:   receive_application_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_RECEIVE_APPLICATION_LAYER_H
#define    LRP_RECEIVE_APPLICATION_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "receive_structures.h"

typedef unsigned char (*LRPReceiveFrameController)(FrameData *const frameData);

void LRP_ReceiveApplicationLayer_controller(LRPReceiveSessionProvider *sessionProvider,
                                            LRPReceiveFrameController *receiveFrameControllerList,
                                            unsigned char receiveFrameControllerListLength);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_RECEIVE_APPLICATION_LAYER_H */

