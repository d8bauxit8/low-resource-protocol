/*
 * File:   transmit_line_code_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_TRANSMIT_LINE_CODE_LAYER_H
#define    LRP_TRANSMIT_LINE_CODE_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../line_code_4b5b.h"
#include "transmit_structures.h"
#include "transmit_link_layer.h"

void
LRP_TransmitLineCodeLayer_handler(_LRPTransmitSessionProvider *sessionProvider,
                                  _LRPLineCode4B5B *lineCode4B5B, unsigned char *data);

void LRP_TransmitLineCodeLayer_startTransmitting(
        _LRPTransmitSessionProvider *sessionProvider,
        _LRPLineCode4B5B *lineCode4B5B, unsigned char *data);

unsigned char LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(
        _LRPTransmitSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_TRANSMIT_LINE_CODE_LAYER_H */

