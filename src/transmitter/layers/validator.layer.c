#include "validator.layer.h"

/**
 * Public method declarations
 */
void LRP_TransmitterValidatorLayer_handler(LRPTransmitterSessionProvider *const sessionProvider) {
    if (sessionProvider->validatorCurrentFrame->status != LRP_TRANSMITTER_FRAME_READY_TO_CHECK) {
        return;
    }

    // TODO: CRC-16 checking

    LRP_Frame_addHeaderDataToFrameBufferFromFrameData(sessionProvider->validatorCurrentFrame);
    LRP_Frame_addDataToFrameBufferFromFrameData(sessionProvider->validatorCurrentFrame);

    LRP_Frame_setStatus(sessionProvider->validatorCurrentFrame, LRP_TRANSMITTER_FRAME_READY_TO_TRANSMITTER);
    sessionProvider->validatorCurrentFrame = sessionProvider->validatorCurrentFrame->next;
}
