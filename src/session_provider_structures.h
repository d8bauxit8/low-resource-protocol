/*
 * File:   session_provider_structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef LRP_SESSION_PROVIDER_STRUCTURES_H
#define    LRP_SESSION_PROVIDER_STRUCTURES_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "frame.h"

typedef struct _LRPSessionProvider {
    _LRPFrame *frameBuffer;
    unsigned char linkLayerStatus;
    unsigned char linkLayerErrorCode;
    unsigned char *deviceId;

    _LRPFrame *linkCurrentFrame;
    _LRPFrame *validatorCurrentFrame;
    _LRPFrame *applicationCurrentFrame;
} _LRPSessionProvider;

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_SESSION_PROVIDER_STRUCTURES_H */