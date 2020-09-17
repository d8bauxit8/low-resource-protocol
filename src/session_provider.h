/*
 * File:   session_provider.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef LRP_SESSION_PROVIDER_H
#define    LRP_SESSION_PROVIDER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "session_provider_structures.h"
#include "link_layer.h"

void LRP_SessionProvider_init(_LRPSessionProvider *sessionProvider, const unsigned char *deviceId,
                              _LRPFrame *frameBuffer,
                              unsigned char frameBufferLength);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_SESSION_PROVIDER_H */