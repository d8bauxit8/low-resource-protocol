#include "receive_layer.h"

void LRP_setReceiveLayerError(_LRPReceiveLayer *const receiveLayer);

void LRP_prepareReceiveLayerToTheNextIteration(_LRPReceiveLayer *const receiveLayer);

unsigned char LRP_isReadLastFrameByte(const unsigned char *const numberOfReceivedBytes);

void LRP_initReceiveLayer(_LRPReceiveLayer *const receiveLayer, const unsigned char *const receiveDeviceId,
                          _LRPFrame *const receiveFrameBuffer,
                          unsigned char receiveFrameBufferLength) {
    LRP_initFrame(receiveFrameBuffer, &receiveFrameBufferLength);

    receiveLayer->receiveDeviceId = receiveDeviceId;
    receiveLayer->frameBuffer = receiveFrameBuffer;
    receiveLayer->frameBufferLength = receiveFrameBufferLength;

    receiveLayer->framingErrorHandler =
    receiveLayer->overrunErrorHandler =
    receiveLayer->parityBitErrorHandler = LRP_noReceiveErrorCallBack;

    LRP_prepareReceiveLayerToTheNextIteration(receiveLayer);
}

void LRP_setFramingErrorHandler(_LRPReceiveLayer *const receiveLayer, _LRPErrorHandler framingErrorHandler) {
    receiveLayer->framingErrorHandler = framingErrorHandler;
}

void LRP_setOverrunErrorHandler(_LRPReceiveLayer *const receiveLayer, _LRPErrorHandler overrunErrorHandler) {
    receiveLayer->overrunErrorHandler = overrunErrorHandler;
}

void LRP_setParityBitErrorHandler(_LRPReceiveLayer *const receiveLayer, _LRPErrorHandler parityBitErrorHandler) {
    receiveLayer->parityBitErrorHandler = parityBitErrorHandler;
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
        receiveLayer->framingErrorHandler();
        LRP_setReceiveLayerError(receiveLayer);
    }

    if (*overrunError) {
        receiveLayer->overrunErrorHandler();
        LRP_setReceiveLayerError(receiveLayer);
    }

    if (LRP_isInvalidParityBit(data, parityBit)) {
        receiveLayer->parityBitErrorHandler();
        LRP_setReceiveLayerError(receiveLayer);
    }

    switch (receiveLayer->numberOfReadBytes) {
        case 1:
            LRP_readTargetDeviceIdAndCommandFromHeader1Data(receiveLayer->currentFrame, &data);
            break;
        case 2:
            LRP_readSourceDeviceIdFromHeader2Data(receiveLayer->currentFrame, &data);
            break;
        default:
            receiveLayer->currentFrame->data[receiveLayer->numberOfReadBytes - 3] = data;
    }

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