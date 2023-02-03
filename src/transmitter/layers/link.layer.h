/* 
 * File:   transmitter-link-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMITTER_LINK_LAYER_H
#define LOW_RESOURCE_PROTOCOL_TRANSMITTER_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../data/session-provider.data.h"
#include "../../link-layer.h"

unsigned char
LRP_TransmitterLinkLayer_handler(LRPTransmitterSessionProvider *sessionProvider);

unsigned char LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(const LRPTransmitterSessionProvider *sessionProvider);

unsigned char LRP_TransmitterLinkLayer_isReadyToTransmitter(const LRPTransmitterSessionProvider *sessionProvider);

void LRP_TransmitterLinkLayer_startTransmitting(LRPTransmitterSessionProvider *sessionProvider);

void LRP_TransmitterLinkLayer_endTransmitting(LRPTransmitterSessionProvider *sessionProvider);

void LRP_TransmitterLinkLayer_errorStatusHandler(LRPTransmitterSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_TRANSMITTER_LINK_LAYER_H */

