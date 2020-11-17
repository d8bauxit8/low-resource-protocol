/*
 * File:   link_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_LINK_LAYER_H
#define    LRP_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "session_provider_structures.h"

#define LINK_LAYER_STATUS_OK 0
#define LINK_LAYER_STATUS_SKIP 1
#define LINK_LAYER_STATUS_ERROR 2

#define LINK_LAYER_NO_ERROR 0
#define LINK_LAYER_NOISE_STROKE_ERROR 1
#define LINK_LAYER_DECODE_ERROR 2

void LRP_LinkLayer_setSkip(_LRPSessionProvider *sessionProvider);

void LRP_LinkLayer_setError(_LRPSessionProvider *sessionProvider, unsigned char errorCode);

void LRP_LinkLayer_setOk(_LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isStatusOK(_LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isStatusError(_LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isError(_LRPSessionProvider *sessionProvider, unsigned char errorCode);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_LINK_LAYER_H */

