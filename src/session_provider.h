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

#include "frame.h"

typedef struct {
    _LRPFrame *frameBuffer;
    unsigned char indexOfReadBytes;
    unsigned char linkLayerStatus;
    unsigned char *deviceId;

    _LRPFrame *linkCurrentFrame;
    _LRPFrame *validatorCurrentFrame;
    _LRPFrame *applicationCurrentFrame;
} _LRPSessionProvider;

void LRP_initSessionProvider(_LRPSessionProvider *const sessionProvider, const unsigned char *const deviceId,
                             _LRPFrame *const frameBuffer,
                             const unsigned char const frameBufferLength);

#ifdef    __cplusplus
}
#endif

#endif    /* SESSION_PROVIDER_H */