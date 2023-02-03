/* 
 * File:   receiver-link-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_RECEIVER_LINK_LAYER_H
#define LOW_RESOURCE_PROTOCOL_RECEIVER_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../data/session-provider.data.h"
#include "../../link-layer.h"

void
LRP_ReceiverLinkLayer_handler(LRPReceiverSessionProvider *sessionProvider, const unsigned char *data);

unsigned char LRP_ReceiverLinkLayer_isStartReceiving(const LRPReceiverSessionProvider *sessionProvider);

void LRP_ReceiverLinkLayer_startReceiving(LRPReceiverSessionProvider *sessionProvider);

void LRP_ReceiverLinkLayer_stopReceiving(LRPReceiverSessionProvider *sessionProvider);

void LRP_ReceiverLinkLayer_errorStatusHandler(LRPReceiverSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_RECEIVER_LINK_LAYER_H */

