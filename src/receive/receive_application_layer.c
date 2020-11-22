#include "receive_application_layer.h"

/**
 * Public method declarations
 */
void LRP_ReceiveApplicationLayer_controller(LRPReceiveSessionProvider *const sessionProvider,
                                            LRPReceiveFrameController *const receiveFrameControllerList,
                                            const unsigned char receiveFrameControllerListLength) {
    if (sessionProvider->applicationCurrentFrame->status != RECEIVE_FRAME_READY_TO_READ) {
        return;
    }

    unsigned char i = 0;
    for (; i < receiveFrameControllerListLength; i++) {
        if (receiveFrameControllerList[i]((FrameData *const) sessionProvider->applicationCurrentFrame)) {
            break;
        }
    }

    LRP_Frame_resetStatus(sessionProvider->applicationCurrentFrame);
    sessionProvider->applicationCurrentFrame = sessionProvider->applicationCurrentFrame->next;
}
