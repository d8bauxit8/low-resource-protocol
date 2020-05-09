#include "receive_link_layer.h"

#define RECEIVE_LINK_LAYER_STATUS_OK 0
#define RECEIVE_LINK_LAYER_STATUS_SKIP 1

/**
 * Public method declarations
 */
void
LRP_receiveLinkLayerHandler(_LRPSessionProvider *const sessionProvider, const unsigned char *const data) {
    switch (sessionProvider->indexOfReadBytes) {
        // It's the header 1
        case 0:
            LRP_readTargetDeviceIdAndCommandFromReceivedByte(sessionProvider->linkCurrentFrame, data);
            if (sessionProvider->linkCurrentFrame->targetDeviceId != *sessionProvider->deviceId &&
                sessionProvider->linkCurrentFrame->targetDeviceId != FRAME_BROADCAST_ID) {
                LRP_setReceiveLinkLayerError(sessionProvider);
                return;
            }
            break;
            // Its the header 2
        case 1:
            LRP_readSourceDeviceIdAndDataLengthFromReceivedByte(sessionProvider->linkCurrentFrame, data);
            break;
    }

    sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfReadBytes] = *data;
    // Increase index of read byte
    sessionProvider->indexOfReadBytes++;
}

void LRP_receiveLinkLayerStartReceiving(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->linkCurrentFrame->status != FRAME_READY_TO_REDEFINE) {
        sessionProvider->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_SKIP;
        return;
    }
    sessionProvider->indexOfReadBytes = 0;
    sessionProvider->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_OK;
    LRP_setFrameStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_IN_RECEIVING);
}

void LRP_receiveLinkLayerEndReceiving(_LRPSessionProvider *const sessionProvider) {
    LRP_setFrameStatus(sessionProvider->linkCurrentFrame, RECEIVE_FRAME_READY_TO_CHECK);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
}

void LRP_setReceiveLinkLayerError(_LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_SKIP;
    LRP_resetFrameStatus(sessionProvider->linkCurrentFrame);
}

unsigned char LRP_isReceiveLinkLayerStatusOK(_LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == RECEIVE_LINK_LAYER_STATUS_OK;
}
