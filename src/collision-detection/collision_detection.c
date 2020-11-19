#include "collision_detection.h"

// Recommend control codes
// https://en.wikipedia.org/wiki/4B5B
// It's a Halt code
#define COLLISION_DETECTION_NOISE_STROKE 0b00100000

void LRP_CollisionDetection_setNoiseStrokeError(LRPCollisionDetection *collisionDetection);

void LRP_CollisionDetection_init(LRPCollisionDetection *const collisionDetection,
                                 LRPTransmitSessionProvider *const transmitSessionProvider,
                                 LRPReceiveSessionProvider *const receiveSessionProvider) {
    collisionDetection->transmitSessionProvider = transmitSessionProvider;
    collisionDetection->receiveSessionProvider = receiveSessionProvider;
}

unsigned char
LRP_CollisionDetection_decodeErrorHandler(LRPCollisionDetection *const collisionDetection,
                                          unsigned char *const data) {
    if (LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection->receiveSessionProvider,
                              LINK_LAYER_DECODE_ERROR)) {
        LRP_CollisionDetection_setNoiseStrokeError(collisionDetection);
        *data = COLLISION_DETECTION_NOISE_STROKE;
        return 1;
    }
    return 0;
}

unsigned char
LRP_CollisionDetection_noiseStrokeErrorHandler(LRPCollisionDetection *const collisionDetection,
                                               const unsigned char *const data) {
    if (*data == COLLISION_DETECTION_NOISE_STROKE) {
        LRP_CollisionDetection_setNoiseStrokeError(collisionDetection);
        return 1;
    }
    return 0;
}

void LRP_CollisionDetection_setNoiseStrokeError(LRPCollisionDetection *const collisionDetection) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection->transmitSessionProvider,
                           LINK_LAYER_NOISE_STROKE_ERROR);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection->receiveSessionProvider,
                           LINK_LAYER_NOISE_STROKE_ERROR);
}