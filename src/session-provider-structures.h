/*
 * File:   session-provider-structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_STRUCTURES_H
#define    LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_STRUCTURES_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "frame.h"

typedef struct LRPSessionProvider {
    LRPFrame *frameBuffer;
    unsigned char linkLayerStatus: 3;
    unsigned char linkLayerErrorCode: 3;
    unsigned char *deviceId;

    LRPFrame *linkCurrentFrame;
    LRPFrame *validatorCurrentFrame;
    LRPFrame *applicationCurrentFrame;
} LRPSessionProvider;

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_STRUCTURES_H */