#include "collision_detection.h"

void LRP_CollisionDetection_init(_LRPCollisionDetection *const collisionDetection,
                                 _LRPTransmitSessionProvider *const transmitSessionProvider,
                                 _LRPReceiveSessionProvider *const receiveSessionProvider) {
    collisionDetection->transmitSessionProvider = transmitSessionProvider;
    collisionDetection->receiveSessionProvider = receiveSessionProvider;
    collisionDetection->readNumberOfNoiseStroke = 0;
}
