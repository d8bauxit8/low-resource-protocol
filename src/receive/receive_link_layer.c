#include "receive_link_layer.h"

/**
 * Public method declarations
 */
void
LRP_ReceiveLinkLayer_handler(_LRPReceiveSessionProvider *const sessionProvider, const unsigned char *const data) {
    if (sessionProvider->indexOfReadBytes == 0) {
        const unsigned char targetDeviceId = LRP_Frame_getTargetDeviceIdFromReceivedByte(data);
        if (targetDeviceId != *sessionProvider->deviceId &&
            targetDeviceId != FRAME_BROADCAST_ID) {
            LRP_LinkLayer_setSkip((_LRPSessionProvider *) sessionProvider);
            LRP_Frame_resetStatus(sessionProvider->linkCurrentFrame);
            return;
        }
    }

    sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfReadBytes] = *data;
    // Increase index of read byte
    sessionProvider->indexOfReadBytes++;
}

void LRP_ReceiveLinkLayer_startReceiving(_LRPReceiveSessionProvider *const sessionProvider) {
    if (sessionProvider->linkCurrentFrame->status != FRAME_READY_TO_REDEFINE) {
        LRP_LinkLayer_setSkip((_LRPSessionProvider *) sessionProvider);
        return;
    }
    sessionProvider->indexOfReadBytes = 0;
    LRP_LinkLayer_setOk((_LRPSessionProvider *) sessionProvider);
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_IN_RECEIVING);
}

void LRP_ReceiveLinkLayer_endReceiving(_LRPSessionProvider *const sessionProvider) {
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_READY_TO_CHECK);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
}
