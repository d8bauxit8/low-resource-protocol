#include "transmit_application_layer.h"

/**
 * Public method declarations
 */
void LRP_setDataIntoReservedFrame(_LRPSessionProvider *const sessionProvider, const unsigned char *const data,
                                  const unsigned char const dataLength) {
    sessionProvider->applicationCurrentFrame->length = dataLength;
    for (unsigned char i = 0; i < dataLength; i++) {
        sessionProvider->applicationCurrentFrame->data[i] = &data[i];
    }
}

void LRP_transmitReservedFrame(_LRPSessionProvider *const sessionProvider,
                               const unsigned char const targetDeviceId, const unsigned char const command) {
    sessionProvider->applicationCurrentFrame->targetDeviceId = targetDeviceId;
    sessionProvider->applicationCurrentFrame->command = command;
    LRP_setFrameStatus(sessionProvider->applicationCurrentFrame, TRANSMIT_FRAME_READY_TO_CHECK);
    sessionProvider->applicationCurrentFrame = sessionProvider->applicationCurrentFrame->next;
}

unsigned char LRP_setReadyToRedefineFrameToReserved(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->applicationCurrentFrame->status == FRAME_READY_TO_REDEFINE) {
        LRP_setFrameStatus(sessionProvider->applicationCurrentFrame, TRANSMIT_FRAME_RESERVED);
        return 1;
    }
    return 0;
}