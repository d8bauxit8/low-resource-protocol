/*
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef RECEIVE_LINE_CODE_LAYER_H
#define    RECEIVE_LINE_CODE_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../line_code_4b5b.h"
#include "receive_structures.h"
#include "receive_link_layer.h"

void LRP_receiveLineCodeLayerHandler(_LRPSessionProvider *const sessionProvider, _LRPLineCode4B5B *const lineCode4B5B,
                                     const unsigned char *const data);

#ifdef    __cplusplus
}
#endif

#endif    /* RECEIVE_LINE_CODE_LAYER_H */

