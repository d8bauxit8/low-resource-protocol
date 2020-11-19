/*
 * File:   collision-detection.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_COLLISION_DETECTION_H
#define    LRP_COLLISION_DETECTION_H

#include "../transmit/transmit_structures.h"
#include "../receive/receive_structures.h"

#ifdef    __cplusplus
extern "C" {
#endif

typedef struct LRPCollisionDetection {
    LRPTransmitSessionProvider *transmitSessionProvider;
    LRPReceiveSessionProvider *receiveSessionProvider;
} LRPCollisionDetection;

void LRP_CollisionDetection_init(LRPCollisionDetection *collisionDetection,
                                 LRPTransmitSessionProvider *transmitSessionProvider,
                                 LRPReceiveSessionProvider *receiveSessionProvider);

unsigned char
LRP_CollisionDetection_decodeErrorHandler(LRPCollisionDetection *collisionDetection, unsigned char *data);

unsigned char
LRP_CollisionDetection_noiseStrokeErrorHandler(LRPCollisionDetection *collisionDetection,
                                               const unsigned char *data);


#ifdef    __cplusplus
}
#endif

#endif    /* LRP_COLLISION_DETECTION_H */

