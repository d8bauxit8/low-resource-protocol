/*
 * File:   session_provider.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef SESSION_PROVIDER_H
#define    SESSION_PROVIDER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "session_provider_structures.h"
#include "link_layer.h"

void LRP_SessionProvider_init(_LRPSessionProvider *const sessionProvider, const unsigned char *const deviceId,
                              _LRPFrame *const frameBuffer,
                              const unsigned char const frameBufferLength);

#ifdef    __cplusplus
}
#endif

#endif    /* SESSION_PROVIDER_H */