/* 
 * File:   receive_link_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef RECEIVE_LINK_LAYER_H
#define    RECEIVE_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "receive_structures.h"
#include "../link_layer.h"

void
LRP_ReceiveLinkLayer_handler(_LRPReceiveSessionProvider *const sessionProvider, const unsigned char *const data);

void LRP_ReceiveLinkLayer_startReceiving(_LRPReceiveSessionProvider *const sessionProvider);

void LRP_ReceiveLinkLayer_endReceiving(_LRPSessionProvider *const sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* RECEIVE_LINK_LAYER_H */

