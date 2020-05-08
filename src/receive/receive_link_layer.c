#include "receive_link_layer.h"

#define RECEIVE_LINK_LAYER_STATUS_OK 0
#define RECEIVE_LINK_LAYER_STATUS_SKIP 1

/**
 * Public method declarations
 */
void
LRP_receiveLinkLayerHandler(_LRPReceiveLayer *const receiveLayer, const unsigned char *const data) {
    switch (receiveLayer->indexOfReadBytes) {
        // It's the header 1
        case 0:
            LRP_readTargetDeviceIdAndCommandFromReceivedByte(receiveLayer->handlerCurrentFrame, data);
            if (receiveLayer->handlerCurrentFrame->targetDeviceId != *receiveLayer->receiveDeviceId &&
                receiveLayer->handlerCurrentFrame->targetDeviceId != FRAME_BROADCAST_ID) {
                LRP_setReceiveLinkLayerError(receiveLayer);
                return;
            }
            break;
            // Its the header 2
        case 1:
            LRP_readSourceDeviceIdAndDataLengthFromReceivedByte(receiveLayer->handlerCurrentFrame, data);
            break;
    }

    receiveLayer->handlerCurrentFrame->buffer[receiveLayer->indexOfReadBytes] = *data;
    // Increase index of read byte
    receiveLayer->indexOfReadBytes++;
}

void LRP_receiveLinkLayerStartReceiving(_LRPReceiveLayer *const receiveLayer) {
    if (receiveLayer->handlerCurrentFrame->status != FRAME_READY_TO_REDEFINE) {
        receiveLayer->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_SKIP;
        return;
    }
    receiveLayer->indexOfReadBytes = 0;
    receiveLayer->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_OK;
    LRP_setFrameStatus(receiveLayer->handlerCurrentFrame, RECEIVE_FRAME_IN_RECEIVING);
}

void LRP_receiveLinkLayerEndReceiving(_LRPReceiveLayer *const receiveLayer) {
    receiveLayer->handlerCurrentFrame->status = RECEIVE_FRAME_READY_TO_CHECK;
    receiveLayer->handlerCurrentFrame = receiveLayer->handlerCurrentFrame->next;
}

void LRP_setReceiveLinkLayerError(_LRPReceiveLayer *const receiveLayer) {
    receiveLayer->linkLayerStatus = RECEIVE_LINK_LAYER_STATUS_SKIP;
    LRP_resetFrameStatus(receiveLayer->handlerCurrentFrame);
}

unsigned char LRP_isReceiveLinkLayerStatusOK(_LRPReceiveLayer *const receiveLayer) {
    return receiveLayer->linkLayerStatus == RECEIVE_LINK_LAYER_STATUS_OK;
}
