/*
 * File:   session-provider.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_H
#define    LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "session-provider-structures.h"
#include "link-layer.h"

void LRP_SessionProvider_init(LRPSessionProvider *sessionProvider, const unsigned char *deviceId,
                              LRPFrame *frameBuffer,
                              unsigned char frameBufferLength);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_H */