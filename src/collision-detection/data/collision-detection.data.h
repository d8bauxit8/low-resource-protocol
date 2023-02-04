/*
 * File:   collision-detection.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_DATA_H
#define LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_DATA_H

#include "../../transmitter/data/session-provider.data.h"
#include "../../receiver/data/session-provider.data.h"

#ifdef    __cplusplus
extern "C" {
#endif

// https://en.wikipedia.org/wiki/4B5B
// It's a Halt code
#define LRP_COLLISION_DETECTION_NOISE_STROKE 0b00100000u

typedef struct LRPCollisionDetection {
    LRPTransmitterSessionProvider *transmitterSessionProvider;
    LRPReceiverSessionProvider *receiverSessionProvider;
    unsigned char backoffTime;
    unsigned char numberOfCollisions;
} LRPCollisionDetection;

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_DATA_H */

