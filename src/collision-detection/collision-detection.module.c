#include "collision-detection.module.h"

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
                                 LRPTransmitterSessionProvider *const transmitterSessionProvider,
                                 LRPReceiverSessionProvider *const receiverSessionProvider) {
    collisionDetection->transmitterSessionProvider = transmitterSessionProvider;
    collisionDetection->receiverSessionProvider = receiverSessionProvider;
    collisionDetection->backoffTime = 0;
    collisionDetection->numberOfCollisions = 0;
}

unsigned char LRP_CollisionDetection_shouldRestartTransmitterModule(LRPCollisionDetection *const collisionDetection) {
    if (collisionDetection->backoffTime == 0) {
        return 1;
    }
    collisionDetection->backoffTime--;
    return 0;
}

unsigned char LRP_CollisionDetection_isDecodeError(const LRPSessionProvider *const sessionProvider) {
    return LRP_LinkLayer_isError(sessionProvider, LRPLinkLayerErrorCode_DecodeError);
}

void LRP_CollisionDetection_decodeErrorHandler(LRPCollisionDetection *const collisionDetection) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection->transmitterSessionProvider,
                           LRPLinkLayerErrorCode_DecodeError);
}

unsigned char LRP_CollisionDetection_isNoiseStrokeError(const unsigned char *const data) {
    return *data == LRP_COLLISION_DETECTION_NOISE_STROKE;
}

void LRP_CollisionDetection_noiseStrokeErrorHandler(LRPCollisionDetection *const collisionDetection) {
    LRP_CollisionDetection_setBackoffTime(collisionDetection);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection->transmitterSessionProvider,
                           LRPLinkLayerErrorCode_NoiseStrokeError);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection->receiverSessionProvider,
                           LRPLinkLayerErrorCode_NoiseStrokeError);
}

unsigned char
LRP_CollisionDetection_isReachedMaxNumberOfCollisions(const LRPCollisionDetection *const collisionDetection) {
    return collisionDetection->numberOfCollisions >= MAX_NUMBER_OF_COLLISION_DETECTION;
}

/**
 * Private method declarations
 */
unsigned char LRP_CollisionDetection_getBackoffTimeMask(const LRPCollisionDetection *const collisionDetection) {
    // https://en.wikipedia.org/wiki/Exponential_backoff
    return ~(0xFF << collisionDetection->numberOfCollisions);
}

void LRP_CollisionDetection_setBackoffTime(LRPCollisionDetection *const collisionDetection) {
    if (!LRP_CollisionDetection_isReachedMaxNumberOfCollisions(collisionDetection)) {
        collisionDetection->numberOfCollisions++;
    }

    collisionDetection->backoffTime = LRP_MSWS_rand() & LRP_CollisionDetection_getBackoffTimeMask(collisionDetection);
}