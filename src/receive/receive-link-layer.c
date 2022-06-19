#include "receive-link-layer.h"

/**
 * Private methods
 */
void LRP_ReceiveLinkLayer_throwFrame(LRPReceiveSessionProvider * sessionProvider);

/**
 * Public method declarations
 */
void
LRP_ReceiveLinkLayer_handler(LRPReceiveSessionProvider *const sessionProvider, const unsigned char *const data) {
    if (sessionProvider->indexOfReadBytes == 0) {
        const unsigned char id = LRP_Frame_getTargetIdFromReceivedByte(data);
        if (LRP_Frame_isGroupIdCommandFromReceivedByte(data)) {
            if (*sessionProvider->groupId != id) {
                LRP_ReceiveLinkLayer_throwFrame(sessionProvider);
                return;
            }
        } else {
            if (id != FRAME_BROADCAST_ID && id != *sessionProvider->deviceId) {
                LRP_ReceiveLinkLayer_throwFrame(sessionProvider);
                return;
            }
        }
    }

    sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfReadBytes] = *data;
    // Increase index of read byte
    sessionProvider->indexOfReadBytes++;
}

unsigned char LRP_ReceiveLinkLayer_isStartReceiving(const LRPReceiveSessionProvider *const sessionProvider) {
    if (sessionProvider->linkCurrentFrame->status != FRAME_READY_TO_REDEFINE ||
        LRP_LinkLayer_isStatusError((LRPSessionProvider *) sessionProvider)) {
        return 0;
    }
    return 1;
}

void LRP_ReceiveLinkLayer_startReceiving(LRPReceiveSessionProvider *const sessionProvider) {
    sessionProvider->indexOfReadBytes = 0;
    LRP_LinkLayer_setOk((LRPSessionProvider *) sessionProvider);
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_IN_RECEIVING);
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

void LRP_ReceiveLinkLayer_throwFrame(LRPReceiveSessionProvider *const sessionProvider) {
    LRP_Frame_resetStatus(sessionProvider->linkCurrentFrame);
    LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
}