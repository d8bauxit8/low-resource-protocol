/*
 * File:   receiver-line-code-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_RECEIVER_LINE_CODE_LAYER_H
#define LOW_RESOURCE_PROTOCOL_RECEIVER_LINE_CODE_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../../line-code-4b5b.h"
#include "../data/session-provider.data.h"
#include "link.layer.h"

void
LRP_ReceiverLineCodeLayer_handler(LRPReceiverSessionProvider *sessionProvider,
                                  LRPLineCode4B5B *lineCode4B5B,
                                  const unsigned char *data);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_RECEIVER_LINE_CODE_LAYER_H */

