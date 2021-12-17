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
#include "../msws.h"

#ifdef    __cplusplus
extern "C" {
#endif

typedef struct LRPCollisionDetection {
    LRPTransmitSessionProvider *transmitSessionProvider;
    LRPReceiveSessionProvider *receiveSessionProvider;
    unsigned char delay;
} LRPCollisionDetection;

void LRP_CollisionDetection_init(LRPCollisionDetection *collisionDetection,
                                 LRPTransmitSessionProvider *transmitSessionProvider,
                                 LRPReceiveSessionProvider *receiveSessionProvider);

unsigned char LRP_CollisionDetection_isRestartTransmitModule(LRPCollisionDetection *collisionDetection);

unsigned char
LRP_CollisionDetection_isDecodeErrorHandler(LRPCollisionDetection *const collisionDetection, unsigned char *const data);

unsigned char
LRP_CollisionDetection_isNoiseStrokeErrorHandler(LRPCollisionDetection *const collisionDetection,
                                                 const unsigned char *const data);


#ifdef    __cplusplus
}
#endif

#endif    /* LRP_COLLISION_DETECTION_H */

