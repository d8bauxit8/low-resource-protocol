/* 
 * File:   transmit_link_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_TRANSMIT_LINK_LAYER_H
#define    LRP_TRANSMIT_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "transmit_structures.h"
#include "../link_layer.h"

unsigned char
LRP_TransmitLinkLayer_handler(_LRPTransmitSessionProvider *const sessionProvider);

unsigned char LRP_TransmitLinkLayer_isEndOfBufferLength(_LRPTransmitSessionProvider *const sessionProvider);

unsigned char LRP_TransmitLinkLayer_isReadyToTransmit(_LRPTransmitSessionProvider *const sessionProvider);

void LRP_TransmitLinkLayer_startTransmitting(_LRPTransmitSessionProvider *const sessionProvider);

void LRP_TransmitLinkLayer_endTransmitting(_LRPSessionProvider *const sessionProvider);


#ifdef    __cplusplus
}
#endif

#endif    /* LRP_TRANSMIT_LINK_LAYER_H */

