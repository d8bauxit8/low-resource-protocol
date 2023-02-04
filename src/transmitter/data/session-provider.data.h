/* 
 * File:   transmitter-structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMITTER_SESSION_PROVIDER_DATA_H
#define LOW_RESOURCE_PROTOCOL_TRANSMITTER_SESSION_PROVIDER_DATA_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../../session-provider.h"

// Status flow
// 1. Ready to redefine
// 2. Reserved
// 3. Ready to check
// 3. Ready to transmitter
// 4. Transmitting
#define LRP_TRANSMITTER_FRAME_RESERVED 1
#define LRP_TRANSMITTER_FRAME_READY_TO_CHECK 2
#define LRP_TRANSMITTER_FRAME_READY_TO_TRANSMITTER 3
#define LRP_TRANSMITTER_FRAME_TRANSMITTING 4

typedef struct LRPTransmitterSessionProvider {
    LRPFrame *frameBuffer;
    LRPLinkLayerStatus linkLayerStatus: 2;
    LRPLinkLayerErrorCode linkLayerErrorCode: 2;
    unsigned char *deviceId;

    LRPFrame *linkCurrentFrame;
    LRPFrame *validatorCurrentFrame;
    LRPFrame *applicationCurrentFrame;
    unsigned char indexOfWrittenBytes;
} LRPTransmitterSessionProvider;

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_TRANSMITTER_SESSION_PROVIDER_DATA_H */


