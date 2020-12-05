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

unsigned char LRP_ReceiveLinkLayer_isStartReceiving(LRPReceiveSessionProvider *const sessionProvider) {
    if (sessionProvider->linkCurrentFrame->status != FRAME_READY_TO_REDEFINE ||
        LRP_LinkLayer_isStatusError((LRPSessionProvider *) sessionProvider)) {
        return 0;
    }

    sessionProvider->indexOfReadBytes = 0;
    LRP_LinkLayer_setOk((LRPSessionProvider *) sessionProvider);
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_IN_RECEIVING);
    return 1;
}

void LRP_ReceiveLinkLayer_stopReceiving(LRPReceiveSessionProvider *const sessionProvider) {
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_READY_TO_CHECK);
    LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
}

void LRP_ReceiveLinkLayer_errorStatusHandler(LRPReceiveSessionProvider *sessionProvider) {
    if (LRP_LinkLayer_isStatusError((LRPSessionProvider *) sessionProvider)) {
        LRP_Frame_resetStatus(sessionProvider->linkCurrentFrame);
        LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
    }
}