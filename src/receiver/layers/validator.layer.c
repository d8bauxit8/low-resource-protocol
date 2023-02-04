#include "validator.layer.h"

/**
 * Public method declarations
 */
void LRP_ReceiverValidatorLayer_handler(LRPReceiverSessionProvider *const sessionProvider) {
    if (sessionProvider->validatorCurrentFrame->status != LRP_RECEIVER_FRAME_READY_TO_CHECK) {
        return;
    }

    // TODO: CRC-16 checking

    LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(sessionProvider->validatorCurrentFrame);
    LRP_Frame_addDataToFrameDataFromFrameBuffer(sessionProvider->validatorCurrentFrame);

    LRP_Frame_setStatus(sessionProvider->validatorCurrentFrame, LRP_RECEIVER_FRAME_READY_TO_READ);
    sessionProvider->validatorCurrentFrame = sessionProvider->validatorCurrentFrame->next;
}