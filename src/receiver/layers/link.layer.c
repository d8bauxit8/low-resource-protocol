#include "link.layer.h"

/**
 * Private methods
 */
void LRP_ReceiverLinkLayer_throwFrame(LRPReceiverSessionProvider *sessionProvider);

/**
 * Public method declarations
 */
void
LRP_ReceiverLinkLayer_handler(LRPReceiverSessionProvider *const sessionProvider, const unsigned char *const data) {
    if (sessionProvider->indexOfReadBytes == 0) {
        const unsigned char id = LRP_Frame_getTargetIdFromReceivedByte(data);
        if (LRP_Frame_isGroupIdCommandFromReceivedByte(data)) {
            if (*sessionProvider->groupId != id) {
                LRP_ReceiverLinkLayer_throwFrame(sessionProvider);
                return;
            }
        } else {
            if (id != LRP_FRAME_BROADCAST_ID && id != *sessionProvider->deviceId) {
                LRP_ReceiverLinkLayer_throwFrame(sessionProvider);
                return;
            }
        }
    }

    sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfReadBytes] = *data;
    // Increase index of read byte
    sessionProvider->indexOfReadBytes++;
}

unsigned char LRP_ReceiverLinkLayer_isStartReceiving(const LRPReceiverSessionProvider *const sessionProvider) {
    if (sessionProvider->linkCurrentFrame->status != LRP_FRAME_READY_TO_REDEFINE ||
        LRP_LinkLayer_isStatusError((LRPSessionProvider *) sessionProvider)) {
        return 0;
    }
    return 1;
}

void LRP_ReceiverLinkLayer_startReceiving(LRPReceiverSessionProvider *const sessionProvider) {
    sessionProvider->indexOfReadBytes = 0;
    LRP_LinkLayer_setOk((LRPSessionProvider *) sessionProvider);
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, LRP_RECEIVER_FRAME_IN_RECEIVING);
}

void LRP_ReceiverLinkLayer_stopReceiving(LRPReceiverSessionProvider *const sessionProvider) {
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, LRP_RECEIVER_FRAME_READY_TO_CHECK);
    LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
}

void LRP_ReceiverLinkLayer_errorStatusHandler(LRPReceiverSessionProvider *sessionProvider) {
    if (LRP_LinkLayer_isStatusError((LRPSessionProvider *) sessionProvider)) {
        LRP_Frame_resetStatus(sessionProvider->linkCurrentFrame);
        LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
    }
}

void LRP_ReceiverLinkLayer_throwFrame(LRPReceiverSessionProvider *const sessionProvider) {
    LRP_Frame_resetStatus(sessionProvider->linkCurrentFrame);
    LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
}