#include "application.layer.h"

/**
 * Public method declarations
 */
void LRP_TransmitterApplicationLayer_setDataIntoReservedFrame(LRPTransmitterSessionProvider *const sessionProvider,
                                                              unsigned char *const data,
                                                              const unsigned char dataLength) {
    unsigned char i = 0;
    sessionProvider->applicationCurrentFrame->length = dataLength;
    for (; i < dataLength; i++) {
        sessionProvider->applicationCurrentFrame->data[i] = &data[i];
    }
}

void LRP_TransmitterApplicationLayer_transmitReservedFrame(LRPTransmitterSessionProvider *const sessionProvider,
                                                           const unsigned char targetId,
                                                           LRPFrameCommand command) {
    sessionProvider->applicationCurrentFrame->targetId = targetId;
    sessionProvider->applicationCurrentFrame->sourceDeviceId = *sessionProvider->deviceId;
    sessionProvider->applicationCurrentFrame->command = command;

    LRP_Frame_setStatus(sessionProvider->applicationCurrentFrame, LRP_TRANSMITTER_FRAME_READY_TO_CHECK);

    sessionProvider->applicationCurrentFrame = sessionProvider->applicationCurrentFrame->next;
}

unsigned char
LRP_TransmitterApplicationLayer_setReadyToRedefineFrameToReserved(
        LRPTransmitterSessionProvider *const sessionProvider) {
    if (sessionProvider->applicationCurrentFrame->status == LRP_FRAME_READY_TO_REDEFINE) {
        LRP_Frame_setStatus(sessionProvider->applicationCurrentFrame, LRP_TRANSMITTER_FRAME_RESERVED);
        return 1;
    }
    return 0;
}