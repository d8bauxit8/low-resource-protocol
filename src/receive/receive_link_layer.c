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
            LRP_readTargetDeviceIdAndCommandFromReceivedByte(sessionProvider->handlerCurrentFrame, data);
            if (sessionProvider->handlerCurrentFrame->targetDeviceId != *sessionProvider->deviceId &&
                sessionProvider->handlerCurrentFrame->targetDeviceId != FRAME_BROADCAST_ID) {
                LRP_setReceiveLinkLayerError(sessionProvider);
                return;
            }
            break;
            // Its the header 2
        case 1:
            LRP_readSourceDeviceIdAndDataLengthFromReceivedByte(sessionProvider->handlerCurrentFrame, data);
            break;
    }

    sessionProvider->handlerCurrentFrame->buffer[sessionProvider->indexOfReadBytes] = *data;
    // Increase index of read byte
    sessionProvider->indexOfReadBytes++;
}

void LRP_receiveLinkLayerStartReceiving(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->handlerCurrentFrame->status != FRAME_READY_TO_REDEFINE) {
        sessionProvider->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_SKIP;
        return;
    }
    sessionProvider->indexOfReadBytes = 0;
    sessionProvider->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_OK;
    LRP_setFrameStatus(sessionProvider->handlerCurrentFrame, RECEIVE_FRAME_IN_RECEIVING);
}

void LRP_receiveLinkLayerEndReceiving(_LRPSessionProvider *const sessionProvider) {
    sessionProvider->handlerCurrentFrame->status = RECEIVE_FRAME_READY_TO_CHECK;
    sessionProvider->handlerCurrentFrame = sessionProvider->handlerCurrentFrame->next;
}

void LRP_setReceiveLinkLayerError(_LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_SKIP;
    LRP_resetFrameStatus(sessionProvider->handlerCurrentFrame);
}

unsigned char LRP_isReceiveLinkLayerStatusOK(_LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == RECEIVE_LINK_LAYER_STATUS_OK;
}
