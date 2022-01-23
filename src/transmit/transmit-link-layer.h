/* 
 * File:   transmit-link-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMIT_LINK_LAYER_H
#define    LOW_RESOURCE_PROTOCOL_TRANSMIT_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "transmit-structures.h"
#include "../link-layer.h"

unsigned char
LRP_TransmitLinkLayer_handler(LRPTransmitSessionProvider *sessionProvider);

unsigned char LRP_TransmitLinkLayer_isUnwrittenDataInBuffer(const LRPTransmitSessionProvider *sessionProvider);

unsigned char LRP_TransmitLinkLayer_isReadyToTransmit(const LRPTransmitSessionProvider *sessionProvider);

void LRP_TransmitLinkLayer_startTransmitting(LRPTransmitSessionProvider *sessionProvider);

void LRP_TransmitLinkLayer_endTransmitting(LRPTransmitSessionProvider *sessionProvider);

void LRP_TransmitLinkLayer_errorStatusHandler(LRPTransmitSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_TRANSMIT_LINK_LAYER_H */

