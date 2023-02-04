/*
 * File:   link-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_LINK_LAYER_H
#define LOW_RESOURCE_PROTOCOL_LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "data/session-provider.data.h"

void LRP_LinkLayer_setSkip(LRPSessionProvider *sessionProvider);

void LRP_LinkLayer_setError(LRPSessionProvider *sessionProvider, unsigned char errorCode);

void LRP_LinkLayer_setOk(LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isStatusOK(const LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isStatusError(const LRPSessionProvider *sessionProvider);

unsigned char LRP_LinkLayer_isError(const LRPSessionProvider *sessionProvider, unsigned char errorCode);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_LINK_LAYER_H */

