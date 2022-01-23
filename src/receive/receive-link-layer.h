/* 
 * File:   receive-link-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_RECEIVE_LINK_LAYER_H
#define    LOW_RESOURCE_PROTOCOL_RECEIVE_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "receive-structures.h"
#include "../link-layer.h"

void
LRP_ReceiveLinkLayer_handler(LRPReceiveSessionProvider *sessionProvider, const unsigned char *data);

unsigned char LRP_ReceiveLinkLayer_isStartReceiving(const LRPReceiveSessionProvider *sessionProvider);

void LRP_ReceiveLinkLayer_startReceiving(LRPReceiveSessionProvider *sessionProvider);

void LRP_ReceiveLinkLayer_stopReceiving(LRPReceiveSessionProvider *sessionProvider);

void LRP_ReceiveLinkLayer_errorStatusHandler(LRPReceiveSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_RECEIVE_LINK_LAYER_H */

