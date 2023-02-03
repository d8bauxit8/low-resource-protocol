/* 
 * File:   receiver-application-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_RECEIVER_APPLICATION_LAYER_H
#define LOW_RESOURCE_PROTOCOL_RECEIVER_APPLICATION_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../data/session-provider.data.h"
#include "../data/frame-controller.data.h"

void LRP_ReceiverApplicationLayer_handler(LRPReceiverSessionProvider *sessionProvider,
                                          const LRPReceiverFrameController *receiverFrameControllerList,
                                          unsigned char numberOfReceiverFrameControllers);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_RECEIVER_APPLICATION_LAYER_H */