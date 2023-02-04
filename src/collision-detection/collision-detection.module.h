/*
 * File:   collision-detection.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_MODULE_H
#define LOW_RESOURCE_PROTOCOL_COLLISION_DETECTION_MODULE_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "./data/collision-detection.data.h"
#include "../utilities/msws.utility.h"

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

