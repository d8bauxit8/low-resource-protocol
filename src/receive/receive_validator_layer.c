#include "receive_validator_layer.h"

/**
 * Public method declarations
 */
void LRP_ReceiveValidatorLayer_handler(_LRPSessionProvider *const sessionProvider) {
    if (sessionProvider->validatorCurrentFrame->status != RECEIVE_FRAME_READY_TO_CHECK) {
        return;
    }

    // TODO: CRC-16 checking

    LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(sessionProvider->validatorCurrentFrame);
    LRP_Frame_addDataToFrameDataFromFrameBuffer(sessionProvider->validatorCurrentFrame);

    LRP_LinkLayer_setSkip(sessionProvider);
    LRP_Frame_setStatus(sessionProvider->validatorCurrentFrame, RECEIVE_FRAME_READY_TO_READ);
    sessionProvider->validatorCurrentFrame = sessionProvider->validatorCurrentFrame->next;
}