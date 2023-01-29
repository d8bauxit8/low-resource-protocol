/*
 * File:   collision-detection.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_H
#define    LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_H

#include "../transmit/transmit-structures.h"
#include "../receive/receive-structures.h"
#include "../msws.h"

#ifdef    __cplusplus
extern "C" {
#endif

// https://en.wikipedia.org/wiki/4B5B
// It's a Halt code
#define COLLISION_DETECTION_NOISE_STROKE 0b00100000u

typedef struct LRPCollisionDetection {
    LRPTransmitSessionProvider *transmitSessionProvider;
    LRPReceiveSessionProvider *receiveSessionProvider;
    unsigned char backoffTime;
    unsigned char numberOfCollisions;
} LRPCollisionDetection;

void LRP_CollisionDetection_init(LRPCollisionDetection *collisionDetection,
                                 LRPTransmitSessionProvider *transmitSessionProvider,
                                 LRPReceiveSessionProvider *receiveSessionProvider);

unsigned char LRP_CollisionDetection_isRestartTransmitModule(LRPCollisionDetection *collisionDetection);

unsigned char LRP_CollisionDetection_isDecodeError(const LRPSessionProvider *sessionProvider);

void LRP_CollisionDetection_decodeErrorHandler(LRPCollisionDetection *collisionDetection);

unsigned char LRP_CollisionDetection_isNoiseStrokeError(const unsigned char *data);

void LRP_CollisionDetection_noiseStrokeErrorHandler(LRPCollisionDetection *collisionDetection);

unsigned char LRP_CollisionDetection_isReachedMaxNumberOfCollisions(const LRPCollisionDetection * collisionDetection);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_H */

