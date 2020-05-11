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

void
LRP_receiveLinkLayerHandler(_LRPSessionProvider *const sessionProvider, const unsigned char *const data);

void LRP_receiveLinkLayerStartReceiving(_LRPSessionProvider *const sessionProvider);

void LRP_receiveLinkLayerEndReceiving(_LRPSessionProvider *const sessionProvider);

void LRP_setReceiveLinkLayerError(_LRPSessionProvider *const sessionProvider);

unsigned char LRP_isReceiveLinkLayerStatusOK(_LRPSessionProvider *const sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* RECEIVE_LINK_LAYER_H */

