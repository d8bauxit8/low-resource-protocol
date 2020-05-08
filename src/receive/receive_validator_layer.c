#include "receive_validator_layer.h"

/**
 * Public method declarations
 */
void LRP_receiveValidatorLinkLayerHandler(_LRPReceiveLayer *const receiveLayer) {
    if (receiveLayer->controllerCurrentFrame->status != RECEIVE_FRAME_READY_TO_CHECK) {
        return;
    }

    // TODO: CRC-16 checking

    LRP_readDataFromBuffer(receiveLayer->controllerCurrentFrame);
    LRP_setFrameStatus(receiveLayer->controllerCurrentFrame, RECEIVE_FRAME_READY_TO_READ);
}
