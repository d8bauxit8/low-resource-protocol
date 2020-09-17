/*
 * File:   receive_line_code_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_RECEIVE_LINE_CODE_LAYER_H
#define    LRP_RECEIVE_LINE_CODE_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../line_code_4b5b.h"
#include "receive_structures.h"
#include "receive_link_layer.h"

void
LRP_ReceiveLineCodeLayer_handler(_LRPReceiveSessionProvider *sessionProvider,
                                 _LRPLineCode4B5B *lineCode4B5B,
                                 const unsigned char *data);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_RECEIVE_LINE_CODE_LAYER_H */

