/*
 * File:   collision-detection.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_MODULE_H
#define LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_MODULE_H

#include "../transmitter/data/session-provider.data.h"
#include "../receiver/data/session-provider.data.h"
#include "../utilities/msws.utility.h"

#ifdef    __cplusplus
extern "C" {
#endif

// https://en.wikipedia.org/wiki/4B5B
// It's a Halt code
#define COLLISION_DETECTION_NOISE_STROKE 0b00100000u

typedef struct LRPCollisionDetection {
    LRPTransmitterSessionProvider *transmitterSessionProvider;
    LRPReceiverSessionProvider *receiverSessionProvider;
    unsigned char backoffTime;
    unsigned char numberOfCollisions;
} LRPCollisionDetection;

void LRP_CollisionDetection_init(LRPCollisionDetection *collisionDetection,
                                 LRPTransmitterSessionProvider *transmitterSessionProvider,
                                 LRPReceiverSessionProvider *receiverSessionProvider);

unsigned char LRP_CollisionDetection_shouldRestartTransmitterModule(LRPCollisionDetection *collisionDetection);

unsigned char LRP_CollisionDetection_isDecodeError(const LRPSessionProvider *sessionProvider);

void LRP_CollisionDetection_decodeErrorHandler(LRPCollisionDetection *collisionDetection);

unsigned char LRP_CollisionDetection_isNoiseStrokeError(const unsigned char *data);

void LRP_CollisionDetection_noiseStrokeErrorHandler(LRPCollisionDetection *collisionDetection);

unsigned char LRP_CollisionDetection_isReachedMaxNumberOfCollisions(const LRPCollisionDetection *collisionDetection);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_MODULE_H */

