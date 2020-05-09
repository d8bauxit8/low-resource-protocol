#include "receive_validator_layer.h"

/**
 * Public method declarations
 */
void LRP_receiveValidatorLinkLayerHandler(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->validatorCurrentFrame->status != RECEIVE_FRAME_READY_TO_CHECK) {
        return;
    }

    // TODO: CRC-16 checking

    LRP_readDataFromBuffer(sessionProvider->validatorCurrentFrame);
    LRP_setFrameStatus(sessionProvider->validatorCurrentFrame, RECEIVE_FRAME_READY_TO_READ);
    sessionProvider->validatorCurrentFrame = sessionProvider->validatorCurrentFrame->next;
}
