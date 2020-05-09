#include "receive_application_layer.h"

/**
 * Public method declarations
 */
void LRP_receiveApplicationLayerController(_LRPSessionProvider *const sessionProvider,
                                           _LRPReceiveFrameController *const receiveFrameControllerList,
                                           const unsigned char const receiveFrameControllerListLength) {
    if (sessionProvider->applicationCurrentFrame->status != RECEIVE_FRAME_READY_TO_READ) {
        return;
    }

    for (unsigned char i = 0; i < receiveFrameControllerListLength; i++) {
        if (receiveFrameControllerList[i]((_FrameData *const) sessionProvider->applicationCurrentFrame)) {
            break;
        }
    }

    LRP_resetFrameStatus(sessionProvider->applicationCurrentFrame);
    sessionProvider->applicationCurrentFrame = sessionProvider->applicationCurrentFrame->next;
}
