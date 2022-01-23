/*
 * File:   transmit-line-code-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMIT_LINE_CODE_LAYER_H
#define    LOW_RESOURCE_PROTOCOL_TRANSMIT_LINE_CODE_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../line-code-4b5b.h"
#include "transmit-structures.h"
#include "transmit-link-layer.h"

void
LRP_TransmitLineCodeLayer_handler(LRPTransmitSessionProvider *sessionProvider,
                                  LRPLineCode4B5B *lineCode4B5B, unsigned char *data);

void LRP_TransmitLineCodeLayer_startTransmitting(
        LRPTransmitSessionProvider *sessionProvider,
        LRPLineCode4B5B *lineCode4B5B, unsigned char *data);

unsigned char LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(
        const LRPTransmitSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_TRANSMIT_LINE_CODE_LAYER_H */

