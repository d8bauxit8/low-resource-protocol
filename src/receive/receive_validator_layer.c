#include "receive_validator_layer.h"

/**
 * Public method declarations
 */
void LRP_receiveValidatorLinkLayerHandler(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->controllerCurrentFrame->status != RECEIVE_FRAME_READY_TO_CHECK) {
        return;
    }

    // TODO: CRC-16 checking

    LRP_readDataFromBuffer(sessionProvider->controllerCurrentFrame);
    LRP_setFrameStatus(sessionProvider->controllerCurrentFrame, RECEIVE_FRAME_READY_TO_READ);
}
