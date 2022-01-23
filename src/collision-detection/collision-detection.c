#include "collision-detection.h"

#define MAX_NUMBER_OF_COLLISION_DETECTION 8u

/**
 * Private method definitions
 */
unsigned char LRP_CollisionDetection_getBackoffTimeMask(const LRPCollisionDetection *collisionDetection);

void LRP_CollisionDetection_setBackoffTime(LRPCollisionDetection *collisionDetection);

/**
 * Public method declarations
 */
void LRP_CollisionDetection_init(LRPCollisionDetection *const collisionDetection,
                                 LRPTransmitSessionProvider *const transmitSessionProvider,
                                 LRPReceiveSessionProvider *const receiveSessionProvider) {
    collisionDetection->transmitSessionProvider = transmitSessionProvider;
    collisionDetection->receiveSessionProvider = receiveSessionProvider;
    collisionDetection->backoffTime = 0;
    collisionDetection->numberOfCollisions = 0;
}

unsigned char LRP_CollisionDetection_isRestartTransmitModule(LRPCollisionDetection *const collisionDetection) {
    if (collisionDetection->backoffTime == 0) {
        return 1;
    }
    collisionDetection->backoffTime--;
    return 0;
}

unsigned char LRP_CollisionDetection_isDecodeError(const LRPSessionProvider *const sessionProvider) {
    return LRP_LinkLayer_isError(sessionProvider, LINK_LAYER_DECODE_ERROR);
}

void LRP_CollisionDetection_decodeErrorHandler(LRPCollisionDetection *const collisionDetection) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection->transmitSessionProvider,
                           LINK_LAYER_DECODE_ERROR);
}

unsigned char LRP_CollisionDetection_isNoiseStrokeError(const unsigned char *const data) {
    return *data == COLLISION_DETECTION_NOISE_STROKE;
}

void LRP_CollisionDetection_noiseStrokeErrorHandler(LRPCollisionDetection *const collisionDetection) {
    LRP_CollisionDetection_setBackoffTime(collisionDetection);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection->transmitSessionProvider,
                           LINK_LAYER_NOISE_STROKE_ERROR);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection->receiveSessionProvider,
                           LINK_LAYER_NOISE_STROKE_ERROR);
}

/**
 * Private method declarations
 */
unsigned char LRP_CollisionDetection_getBackoffTimeMask(const LRPCollisionDetection *const collisionDetection) {
    // https://en.wikipedia.org/wiki/Exponential_backoff
    return ~(0xFF << collisionDetection->numberOfCollisions);
}

void LRP_CollisionDetection_setBackoffTime(LRPCollisionDetection *const collisionDetection) {
    if (collisionDetection->numberOfCollisions != MAX_NUMBER_OF_COLLISION_DETECTION) {
        collisionDetection->numberOfCollisions++;
    }

    collisionDetection->backoffTime = LRP_MSVS_rand() & LRP_CollisionDetection_getBackoffTimeMask(collisionDetection);
}