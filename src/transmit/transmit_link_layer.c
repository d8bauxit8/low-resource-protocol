#include "transmit_link_layer.h"

#define TRANSMIT_LINK_LAYER_STATUS_OK 0
#define TRANSMIT_LINK_LAYER_STATUS_SKIP 1

/**
 * Public method declarations
 */
unsigned char
LRP_transmitLinkLayerHandler(_LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfReadBytes++];
}

void LRP_transmitLinkLayerStartTransmitting(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->linkCurrentFrame->status != TRANSMIT_FRAME_READY_TO_TRANSMIT) {
        sessionProvider->linkLayerStatus = TRANSMIT_LINK_LAYER_STATUS_SKIP;
        return;
    }
    sessionProvider->indexOfReadBytes = 0;
    sessionProvider->linkLayerStatus = TRANSMIT_LINK_LAYER_STATUS_OK;
    LRP_setFrameStatus(sessionProvider->linkCurrentFrame, TRANSMIT_FRAME_TRANSMITTING);
}

void LRP_transmitLinkLayerEndTransmitting(_LRPSessionProvider *const sessionProvider) {
    LRP_setFrameStatus(sessionProvider->linkCurrentFrame, FRAME_READY_TO_REDEFINE);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
}

unsigned char LRP_isTransmitLinkLayerStatusOK(_LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == TRANSMIT_LINK_LAYER_STATUS_OK;
}
