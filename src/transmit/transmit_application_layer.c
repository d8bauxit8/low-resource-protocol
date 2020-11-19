#include "transmit_application_layer.h"

/**
 * Public method declarations
 */
void LRP_TransmitApplicationLayer_setDataIntoReservedFrame(LRPSessionProvider *const sessionProvider,
                                                           unsigned char *const data,
                                                           const unsigned char dataLength) {
    sessionProvider->applicationCurrentFrame->length = dataLength;
    for (unsigned char i = 0; i < dataLength; i++) {
        sessionProvider->applicationCurrentFrame->data[i] = &data[i];
    }
}

void LRP_TransmitApplicationLayer_transmitReservedFrame(LRPSessionProvider *const sessionProvider,
                                                        const unsigned char targetDeviceId,
                                                        const unsigned char command) {
    sessionProvider->applicationCurrentFrame->targetDeviceId = targetDeviceId;
    sessionProvider->applicationCurrentFrame->sourceDeviceId = *sessionProvider->deviceId;
    sessionProvider->applicationCurrentFrame->command = command;

    LRP_Frame_setStatus(sessionProvider->applicationCurrentFrame, TRANSMIT_FRAME_READY_TO_CHECK);

    sessionProvider->applicationCurrentFrame = sessionProvider->applicationCurrentFrame->next;
}

unsigned char
LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->applicationCurrentFrame->status == FRAME_READY_TO_REDEFINE) {
        LRP_Frame_setStatus(sessionProvider->applicationCurrentFrame, TRANSMIT_FRAME_RESERVED);
        return 1;
    }
    return 0;
}