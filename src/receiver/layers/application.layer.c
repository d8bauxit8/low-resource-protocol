#include "application.layer.h"

/**
 * Public method declarations
 */
void LRP_ReceiverApplicationLayer_handler(LRPReceiverSessionProvider *const sessionProvider,
                                          const LRPReceiverFrameController *const receiverFrameControllerList,
                                          const unsigned char numberOfReceiverFrameControllers) {
    if (sessionProvider->applicationCurrentFrame->status != RECEIVER_FRAME_READY_TO_READ) {
        return;
    }

    unsigned char i = 0;
    for (; i < numberOfReceiverFrameControllers; i++) {
        if ((*receiverFrameControllerList[i])((const LRPFrameData *const) sessionProvider->applicationCurrentFrame)) {
            break;
        }
    }

    LRP_Frame_resetStatus(sessionProvider->applicationCurrentFrame);
    sessionProvider->applicationCurrentFrame = sessionProvider->applicationCurrentFrame->next;
}
