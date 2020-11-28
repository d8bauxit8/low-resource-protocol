/* 
 * File:   receive_link_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_RECEIVE_LINK_LAYER_H
#define    LRP_RECEIVE_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "receive_structures.h"
#include "../link_layer.h"

void
LRP_ReceiveLinkLayer_handler(LRPReceiveSessionProvider *sessionProvider, const unsigned char *data);

unsigned char LRP_ReceiveLinkLayer_isStartReceiving(LRPReceiveSessionProvider *sessionProvider);

void LRP_ReceiveLinkLayer_stopReceiving(LRPReceiveSessionProvider *sessionProvider);

void LRP_ReceiveLinkLayer_errorStatusHandler(LRPReceiveSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_RECEIVE_LINK_LAYER_H */

