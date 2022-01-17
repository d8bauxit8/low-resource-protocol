/*
 * File:   link-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_LINK_LAYER_H
#define    LOW_RESOURCE_PROTOCOL_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "session-provider-structures.h"

#define LINK_LAYER_STATUS_OK 0
#define LINK_LAYER_STATUS_SKIP 1
#define LINK_LAYER_STATUS_ERROR 2

#define LINK_LAYER_NO_ERROR 0
#define LINK_LAYER_NOISE_STROKE_ERROR 1
#define LINK_LAYER_DECODE_ERROR 2
#define LINK_LAYER_INTERNAL_ERROR 3

void LRP_LinkLayer_setSkip(LRPSessionProvider *sessionProvider);

void LRP_LinkLayer_setError(LRPSessionProvider *sessionProvider, const unsigned char errorCode);

void LRP_LinkLayer_setOk(LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isStatusOK(LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isStatusError(LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isError(LRPSessionProvider *sessionProvider, const unsigned char errorCode);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_LINK_LAYER_H */

