#include "receive_application_layer.h"

/**
 * Public method declarations
 */
void LRP_initReceiveApplicationLayer(_LRPReceiveLayer *const receiveLayer, const unsigned char *const receiveDeviceId,
                                     _LRPFrame *const receiveFrameBuffer,
                                     const unsigned char const receiveFrameBufferLength) {
    LRP_initFrameBuffer(receiveFrameBuffer, &receiveFrameBufferLength);

    receiveLayer->receiveDeviceId = receiveDeviceId;

    receiveLayer->frameBuffer = receiveFrameBuffer;

    receiveLayer->handlerCurrentFrame =
    receiveLayer->controllerCurrentFrame = &receiveLayer->frameBuffer[0];

    receiveLayer->indexOfReadBytes = 0;
}

void LRP_receiveApplicationLayerController(_LRPReceiveLayer *const receiveLayer,
                                           _LRPReceiveFrameController *const receiveFrameControllerList,
                                           const unsigned char const receiveFrameControllerListLength) {
    if (receiveLayer->controllerCurrentFrame->status != RECEIVE_FRAME_READY_TO_READ) {
        return;
    }

    for (unsigned char i = 0; i < receiveFrameControllerListLength; i++) {
        if (receiveFrameControllerList[i]((_FrameData *const) receiveLayer->controllerCurrentFrame)) {
            break;
        }
    }

    LRP_resetFrameStatus(receiveLayer->controllerCurrentFrame);
    receiveLayer->controllerCurrentFrame = receiveLayer->controllerCurrentFrame->next;
}
