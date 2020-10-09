#include "collision_detection.h"

#define COLLISION_DETECTION_FIRST_NOISE_STROKE 1
#define COLLISION_DETECTION_SECOND_NOISE_STROKE 2

// Recommend control codes
// https://en.wikipedia.org/wiki/4B5B
// It's a Halt code
#define COLLISION_DETECTION_NOISE_STROKE 0b00100000

void LRP_CollisionDetection_setErrorToReceiveModule(_LRPReceiveSessionProvider *receiveSessionProvider);

void LRP_CollisionDetection_setErrorToTransmitModule(_LRPTransmitSessionProvider *transmitSessionProvider);

void LRP_CollisionDetection_init(_LRPCollisionDetection *const collisionDetection,
                                 _LRPTransmitSessionProvider *const transmitSessionProvider,
                                 _LRPReceiveSessionProvider *const receiveSessionProvider) {
    collisionDetection->transmitSessionProvider = transmitSessionProvider;
    collisionDetection->receiveSessionProvider = receiveSessionProvider;
    collisionDetection->readNumberOfNoiseStroke = 0;
}

void LRP_CollisionDetection_errorController(_LRPCollisionDetection *const collisionDetection) {
    if (LRP_LinkLayer_isStatusError((_LRPSessionProvider *) collisionDetection->receiveSessionProvider) ||
        LRP_LinkLayer_isStatusError((_LRPSessionProvider *) collisionDetection->transmitSessionProvider)) {
        LRP_CollisionDetection_setErrorToReceiveModule(collisionDetection->receiveSessionProvider);
        LRP_CollisionDetection_setErrorToTransmitModule(collisionDetection->transmitSessionProvider);
    }
}

void
LRP_CollisionDetection_receiveNoiseStrokeController(_LRPCollisionDetection *const collisionDetection,
                                                    const unsigned char *const data) {
    if (*data == COLLISION_DETECTION_NOISE_STROKE) {
        collisionDetection->readNumberOfNoiseStroke++;
        switch (collisionDetection->readNumberOfNoiseStroke) {
            case COLLISION_DETECTION_FIRST_NOISE_STROKE:
                LRP_CollisionDetection_setErrorToReceiveModule(collisionDetection->receiveSessionProvider);
                LRP_CollisionDetection_setErrorToTransmitModule(collisionDetection->transmitSessionProvider);
                break;
            case COLLISION_DETECTION_SECOND_NOISE_STROKE:
            default:
                collisionDetection->readNumberOfNoiseStroke = 0;
        }
    }
}

/**
 * Private functions
 */
void LRP_CollisionDetection_setErrorToReceiveModule(_LRPReceiveSessionProvider *const receiveSessionProvider) {
    LRP_LinkLayer_setError((_LRPSessionProvider *) receiveSessionProvider);
    LRP_Frame_resetStatus(receiveSessionProvider->linkCurrentFrame);
}

void LRP_CollisionDetection_setErrorToTransmitModule(_LRPTransmitSessionProvider *const transmitSessionProvider) {
    LRP_LinkLayer_setError((_LRPSessionProvider *) transmitSessionProvider);
    if (transmitSessionProvider->linkCurrentFrame->status ==
        TRANSMIT_FRAME_TRANSMITTING) {
        LRP_Frame_setStatus(transmitSessionProvider->linkCurrentFrame,
                            TRANSMIT_FRAME_READY_TO_TRANSMIT);
    }
}