#include "transmit_validator_layer.h"

/**
 * Public method declarations
 */
void LRP_transmitValidatorLayerHandler(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->validatorCurrentFrame->status != TRANSMIT_FRAME_READY_TO_CHECK) {
        return;
    }

    // TODO: CRC-16 checking

    LRP_addHeaderDataToFrameBufferFromFrameData(sessionProvider->validatorCurrentFrame);
    LRP_addDataToFrameBufferFromFrameData(sessionProvider->validatorCurrentFrame);

    LRP_setFrameStatus(sessionProvider->validatorCurrentFrame, TRANSMIT_FRAME_READY_TO_TRANSMIT);
}
