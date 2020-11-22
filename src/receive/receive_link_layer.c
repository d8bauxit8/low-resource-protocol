#include "receive_link_layer.h"

/**
 * Public method declarations
 */
void
LRP_ReceiveLinkLayer_handler(LRPReceiveSessionProvider *const sessionProvider, const unsigned char *const data) {
    if (sessionProvider->indexOfReadBytes == 0) {
        const unsigned char targetDeviceId = LRP_Frame_getTargetDeviceIdFromReceivedByte(data);
        if (targetDeviceId != *sessionProvider->deviceId &&
            targetDeviceId != FRAME_BROADCAST_ID) {
            LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
            LRP_Frame_resetStatus(sessionProvider->linkCurrentFrame);
            return;
        }
    }

    sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfReadBytes] = *data;
    // Increase index of read byte
    sessionProvider->indexOfReadBytes++;
}

void LRP_ReceiveLinkLayer_startReceiving(LRPReceiveSessionProvider *const sessionProvider) {
    if (sessionProvider->linkCurrentFrame->status != FRAME_READY_TO_REDEFINE) {
        return;
    }
    sessionProvider->indexOfReadBytes = 0;
    LRP_LinkLayer_setOk((LRPSessionProvider *) sessionProvider);
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_IN_RECEIVING);
}

void LRP_ReceiveLinkLayer_endReceiving(LRPReceiveSessionProvider *const sessionProvider) {
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_READY_TO_CHECK);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
}

void LRP_ReceiveLinkLayer_errorStatusHandler(LRPReceiveSessionProvider *sessionProvider) {
    if (LRP_LinkLayer_isStatusError((LRPSessionProvider *) sessionProvider)) {
        LRP_Frame_resetStatus(sessionProvider->linkCurrentFrame);
        LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
    }
}