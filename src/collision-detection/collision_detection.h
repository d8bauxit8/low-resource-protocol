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

typedef struct _LRPCollisionDetection {
    _LRPTransmitSessionProvider *transmitSessionProvider;
    _LRPReceiveSessionProvider *receiveSessionProvider;
} _LRPCollisionDetection;

void LRP_CollisionDetection_init(_LRPCollisionDetection *collisionDetection,
                                 _LRPTransmitSessionProvider *transmitSessionProvider,
                                 _LRPReceiveSessionProvider *receiveSessionProvider);

unsigned char
LRP_CollisionDetection_decodeErrorHandler(_LRPCollisionDetection *collisionDetection, unsigned char *data);

unsigned char
LRP_CollisionDetection_noiseStrokeErrorHandler(_LRPCollisionDetection *collisionDetection,
                                               const unsigned char *data);


#ifdef    __cplusplus
}
#endif

#endif    /* LRP_COLLISION_DETECTION_H */

