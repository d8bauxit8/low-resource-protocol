/*
 * File:   receiver-structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_RECEIVER_SESSION_PROVIDER_DATA_H
#define LOW_RESOURCE_PROTOCOL_RECEIVER_SESSION_PROVIDER_DATA_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../../session-provider.h"

// Status flow
// 1. In receiving
// 2. Ready to check
// 3. Ready to read
// 4. Ready to redefine
#define LRP_RECEIVER_FRAME_IN_RECEIVING 1
#define LRP_RECEIVER_FRAME_READY_TO_CHECK 2
#define LRP_RECEIVER_FRAME_READY_TO_READ 3

typedef struct LRPReceiverSessionProvider {
    LRPFrame *frameBuffer;
    unsigned char linkLayerStatus: 2;
    unsigned char linkLayerErrorCode: 2;
    unsigned char *deviceId;

    LRPFrame *linkCurrentFrame;
    LRPFrame *validatorCurrentFrame;
    LRPFrame *applicationCurrentFrame;
    unsigned char indexOfReadBytes;
    unsigned char *groupId;
} LRPReceiverSessionProvider;

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_RECEIVER_SESSION_PROVIDER_DATA_H */


