#include "collision_detection.h"

#define COLLISION_DETECTION_FIRST_NOISE_STROKE 1
#define COLLISION_DETECTION_SECOND_NOISE_STROKE 2

// Recommend control codes
// https://en.wikipedia.org/wiki/4B5B
// It's a Halt code
#define COLLISION_DETECTION_NOISE_STROKE 0b00100000

void LRP_CollisionDetection_init(_LRPCollisionDetection *const collisionDetection,
                                 _LRPTransmitSessionProvider *const transmitSessionProvider,
                                 _LRPReceiveSessionProvider *const receiveSessionProvider) {
    collisionDetection->transmitSessionProvider = transmitSessionProvider;
    collisionDetection->receiveSessionProvider = receiveSessionProvider;
    collisionDetection->readNumberOfNoiseStroke = 0;
}

void
LRP_CollisionDetection_slaveController(_LRPCollisionDetection *const collisionDetection,
                                       const unsigned char *const data) {
    if (*data == COLLISION_DETECTION_NOISE_STROKE) {
        collisionDetection->readNumberOfNoiseStroke++;
        switch (collisionDetection->readNumberOfNoiseStroke) {
            case COLLISION_DETECTION_FIRST_NOISE_STROKE:
                LRP_LinkLayer_setSkip((_LRPSessionProvider *) collisionDetection->transmitSessionProvider);
                LRP_LinkLayer_setSkip((_LRPSessionProvider *) collisionDetection->receiveSessionProvider);
                break;
            case COLLISION_DETECTION_SECOND_NOISE_STROKE:
                LRP_Frame_resetStatus(collisionDetection->receiveSessionProvider->linkCurrentFrame);
                if (collisionDetection->transmitSessionProvider->linkCurrentFrame->status ==
                    TRANSMIT_FRAME_TRANSMITTING) {
                    LRP_Frame_setStatus(collisionDetection->transmitSessionProvider->linkCurrentFrame,
                                        TRANSMIT_FRAME_READY_TO_TRANSMIT);
                }
            default:
                collisionDetection->readNumberOfNoiseStroke = 0;
        }
    }
}
