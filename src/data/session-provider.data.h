/*
 * File:   session-provider-structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_DATA_H
#define LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_DATA_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../frame.h"
#include "link-layer.data.h"

typedef struct LRPSessionProvider {
    const LRPFrame *frameBuffer;
    unsigned char linkLayerStatus: 2;
    unsigned char linkLayerErrorCode: 2;
    unsigned char *deviceId;

    const LRPFrame *linkCurrentFrame;
    const LRPFrame *validatorCurrentFrame;
    const LRPFrame *applicationCurrentFrame;
} LRPSessionProvider;

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_SESSION_PROVIDER_DATA_H */