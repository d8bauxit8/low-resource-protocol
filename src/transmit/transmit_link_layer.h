/* 
 * File:   transmit_link_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef TRANSMIT_LINK_LAYER_H
#define    TRANSMIT_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "transmit_structures.h"

unsigned char
LRP_transmitLinkLayerHandler(_LRPSessionProvider *const sessionProvider);

void LRP_transmitLinkLayerStartTransmitting(_LRPSessionProvider *const sessionProvider);

void LRP_transmitLinkLayerEndTransmitting(_LRPSessionProvider *const sessionProvider);

unsigned char LRP_isTransmitLinkLayerStatusOK(_LRPSessionProvider *const sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* TRANSMIT_LINK_LAYER_H */

