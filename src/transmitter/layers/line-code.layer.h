/*
 * File:   transmitter-line-code-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMITTER_LINE_CODE_LAYER_H
#define LOW_RESOURCE_PROTOCOL_TRANSMITTER_LINE_CODE_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../../line-code-4b5b.h"
#include "../data/session-provider.data.h"
#include "link.layer.h"

void
LRP_TransmitterLineCodeLayer_handler(LRPTransmitterSessionProvider *sessionProvider,
                                     LRPLineCode4B5B *lineCode4B5B, unsigned char *data);

void LRP_TransmitterLineCodeLayer_startTransmitting(
        LRPTransmitterSessionProvider *sessionProvider,
        LRPLineCode4B5B *lineCode4B5B, unsigned char *data);

unsigned char LRP_TransmitterLineCodeLayer_isReadyToStartTransmitting(
        const LRPTransmitterSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_TRANSMITTER_LINE_CODE_LAYER_H */

