/* 
 * File:   layers.h
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
LRP_receiveLinkLayerHandler(_LRPReceiveLayer *const receiveLayer, const unsigned char *const data);

void LRP_receiveLinkLayerStartReceiving(_LRPReceiveLayer *const receiveLayer);

void LRP_receiveLinkLayerEndReceiving(_LRPReceiveLayer *const receiveLayer);

void LRP_setReceiveLinkLayerError(_LRPReceiveLayer *const receiveLayer);

unsigned char LRP_isReceiveLinkLayerStatusOK(_LRPReceiveLayer *const receiveLayer);

#ifdef    __cplusplus
}
#endif

#endif    /* RECEIVE_LINK_LAYER_H */

