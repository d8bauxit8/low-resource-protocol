#include "transmit_validator_layer.h"

/**
 * Public method declarations
 */
void LRP_TransmitValidatorLayer_handler(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->validatorCurrentFrame->status != TRANSMIT_FRAME_READY_TO_CHECK) {
        return;
    }

    // TODO: CRC-16 checking

    LRP_Frame_addHeaderDataToFrameBufferFromFrameData(sessionProvider->validatorCurrentFrame);
    LRP_Frame_addDataToFrameBufferFromFrameData(sessionProvider->validatorCurrentFrame);

    LRP_LinkLayer_setSkip(sessionProvider);
    LRP_Frame_setStatus(sessionProvider->validatorCurrentFrame, TRANSMIT_FRAME_READY_TO_TRANSMIT);
    sessionProvider->validatorCurrentFrame = sessionProvider->validatorCurrentFrame->next;
}
