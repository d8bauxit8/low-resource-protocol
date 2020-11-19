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
LRP_ReceiveLinkLayer_handler(_LRPReceiveSessionProvider *sessionProvider, const unsigned char *data);

void LRP_ReceiveLinkLayer_startReceiving(_LRPReceiveSessionProvider *sessionProvider);

void LRP_ReceiveLinkLayer_endReceiving(LRPSessionProvider *sessionProvider);

void LRP_ReceiveLinkLayer_errorStatusHandler(LRPSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_RECEIVE_LINK_LAYER_H */

