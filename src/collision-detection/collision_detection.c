#include "collision_detection.h"

// Recommend control codes
// https://en.wikipedia.org/wiki/4B5B
// It's a Halt code
#define COLLISION_DETECTION_NOISE_STROKE 0b00100000u
#define COLLISION_DETECTION_DELAY_MASK 0b00000001u

void LRP_CollisionDetection_setNoiseStrokeError(LRPCollisionDetection *collisionDetection);

void LRP_CollisionDetection_init(LRPCollisionDetection *const collisionDetection,
                                 LRPTransmitSessionProvider *const transmitSessionProvider,
                                 LRPReceiveSessionProvider *const receiveSessionProvider) {
    collisionDetection->transmitSessionProvider = transmitSessionProvider;
    collisionDetection->receiveSessionProvider = receiveSessionProvider;
    collisionDetection->delay = 0;
}

unsigned char LRP_CollisionDetection_isRestartTransmitModule(LRPCollisionDetection *collisionDetection) {
    if ((collisionDetection->delay & COLLISION_DETECTION_DELAY_MASK) == 0) {
        return 1;
    }

    collisionDetection->delay >>= 1u;
    return 0;
}

unsigned char
LRP_CollisionDetection_isDecodeErrorHandler(LRPCollisionDetection *const collisionDetection,
                                            unsigned char *const data) {
    if (LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection->receiveSessionProvider,
                              LINK_LAYER_DECODE_ERROR)) {
        LRP_CollisionDetection_setNoiseStrokeError(collisionDetection);
        collisionDetection->delay = LRP_MSVS_rand();
        *data = COLLISION_DETECTION_NOISE_STROKE;
        return 1;
    }
    return 0;
}

unsigned char
LRP_CollisionDetection_isNoiseStrokeErrorHandler(LRPCollisionDetection *const collisionDetection,
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