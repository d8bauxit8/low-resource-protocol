#include "receive-application-layer.h"

/**
 * Public method declarations
 */
void LRP_ReceiveApplicationLayer_handler(LRPReceiveSessionProvider *const sessionProvider,
                                         const LRPReceiveFrameController *const receiveFrameControllerList,
                                         const unsigned char numberOfReceiveFrameControllers) {
    if (sessionProvider->applicationCurrentFrame->status != RECEIVE_FRAME_READY_TO_READ) {
        return;
    }

    unsigned char i = 0;
    for (; i < numberOfReceiveFrameControllers; i++) {
        if ((*receiveFrameControllerList[i])((const LRPFrameData *const) sessionProvider->applicationCurrentFrame)) {
            break;
        }
    }

    LRP_Frame_resetStatus(sessionProvider->applicationCurrentFrame);
    sessionProvider->applicationCurrentFrame = sessionProvider->applicationCurrentFrame->next;
}
