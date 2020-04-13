#include "receive_layer.h"

void LRP_setReceiveLayerError(_LRPReceiveLayer *const receiveLayer);

void LRP_prepareReceiveLayerToTheNextIteration(_LRPReceiveLayer *const receiveLayer);

unsigned char LRP_isReadLastFrameByte(const unsigned char *const numberOfReceivedBytes);

void LRP_initReceiveLayer(_LRPReceiveLayer *const receiveLayer, const unsigned char *const receiveDeviceId,
                          _LRPFrame *const receiveFrameBuffer,
                          unsigned char frameBufferLength,
                          void (*LRP_framingErrorHandler)(void),
                          void (*LRP_overrunErrorHandler)(void),
                          void (*LRP_parityBitErrorHandler)(void)) {
    LRP_initFrame(receiveFrameBuffer, &frameBufferLength);

    receiveLayer->receiveDeviceId = receiveDeviceId;
    receiveLayer->frameBuffer = receiveFrameBuffer;
    receiveLayer->frameBufferLength = frameBufferLength;

    receiveLayer->LRP_framingErrorHandler = LRP_framingErrorHandler;
    receiveLayer->LRP_overrunErrorHandler = LRP_overrunErrorHandler;
    receiveLayer->LRP_parityBitErrorHandler = LRP_parityBitErrorHandler;

    LRP_prepareReceiveLayerToTheNextIteration(receiveLayer);
}

void
LRP_receiveLayerController(_LRPReceiveLayer *const receiveLayer, unsigned char data,
                           const unsigned char *const parityBit,
                           const unsigned char *const framingError,
                           const unsigned char *const overrunError) {
    receiveLayer->numberOfReadBytes++;

    if (receiveLayer->status == RECEIVE_LAYER_STATUS_ERROR) {
        if (LRP_isReadLastFrameByte(&receiveLayer->numberOfReadBytes)) {
            LRP_prepareReceiveLayerToTheNextIteration(receiveLayer);
        }
        return;
    }

    if (*framingError) {
        receiveLayer->LRP_framingErrorHandler();
        LRP_setReceiveLayerError(receiveLayer);
    }

    if (*overrunError) {
        receiveLayer->LRP_overrunErrorHandler();
        LRP_setReceiveLayerError(receiveLayer);
    }

    if (LRP_isInvalidParityBit(data, parityBit)) {
        receiveLayer->LRP_parityBitErrorHandler();
        LRP_setReceiveLayerError(receiveLayer);
    }

    LRP_receiveFrameController(receiveLayer->currentFrame, &receiveLayer->numberOfReadBytes, data);

    if (receiveLayer->currentFrame->targetDeviceId != *receiveLayer->receiveDeviceId) {
        LRP_setReceiveLayerError(receiveLayer);
    }

    if (LRP_isReadLastFrameByte(&receiveLayer->numberOfReadBytes)) {
        if (receiveLayer->status == RECEIVE_LAYER_STATUS_OK) {
            receiveLayer->currentFrame->status = RECEIVE_FRAME_COMPLETED;
        }
        LRP_prepareReceiveLayerToTheNextIteration(receiveLayer);
    }
}

void LRP_noReceiveErrorCallBack(void) {}

void LRP_setReceiveLayerError(_LRPReceiveLayer *const receiveLayer) {
    receiveLayer->status = RECEIVE_LAYER_STATUS_ERROR;
    LRP_resetFrame(receiveLayer->currentFrame);
}

void LRP_prepareReceiveLayerToTheNextIteration(_LRPReceiveLayer *const receiveLayer) {
    char currentFrameIndex = LRP_findFirstFrameIndexByStatus(receiveLayer->frameBuffer,
                                                             &receiveLayer->frameBufferLength,
                                                             FRAME_READY_TO_REDEFINE);
    receiveLayer->numberOfReadBytes = 0;
    receiveLayer->status = RECEIVE_LAYER_STATUS_OK;

    if (currentFrameIndex == -1) {
        receiveLayer->status = RECEIVE_LAYER_STATUS_ERROR;
        return;
    }

    receiveLayer->currentFrame = &receiveLayer->frameBuffer[currentFrameIndex];
    receiveLayer->currentFrame->status = FRAME_IN_PROGRESS;
}

unsigned char LRP_isReadLastFrameByte(const unsigned char *const numberOfReceivedBytes) {
    if (*numberOfReceivedBytes == FRAME_LENGTH) {
        return 1;
    }
    return 0;
}