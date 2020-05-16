/*
 * File:   transmit_line_code_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef TRANSMIT_LINE_CODE_LAYER_H
#define    TRANSMIT_LINE_CODE_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../line_code_4b5b.h"
#include "transmit_structures.h"
#include "transmit_link_layer.h"

unsigned char
LRP_TransmitLineCodeLayer_handler(_LRPTransmitSessionProvider *const sessionProvider,
                                  _LRPLineCode4B5B *const lineCode4B5B);

#ifdef    __cplusplus
}
#endif

#endif    /* TRANSMIT_LINE_CODE_LAYER_H */

