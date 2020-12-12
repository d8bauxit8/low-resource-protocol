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

typedef struct LRPSessionProvider {
    LRPFrame *frameBuffer;
    unsigned char linkLayerStatus: 2;
    unsigned char linkLayerErrorCode;
    unsigned char *deviceId;

    LRPFrame *linkCurrentFrame;
    LRPFrame *validatorCurrentFrame;
    LRPFrame *applicationCurrentFrame;
} LRPSessionProvider;

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_SESSION_PROVIDER_STRUCTURES_H */