#include "receive_layer.h"

void LRP_setReceiveLayerError(_LRPReceiveLayer *const receiveLayer);

void LRP_prepareReceiveLayerToTheNextIteration(_LRPReceiveLayer *const receiveLayer);

unsigned char LRP_isReadLastFrameByte(const unsigned char *const numberOfReceivedBytes);

void LRP_resetReceiveLayer(_LRPReceiveLayer *const receiveLayer);

void LRP_initReceiveLayer(_LRPReceiveLayer *const receiveLayer, const unsigned char *const receiveDeviceId,
                          _LRPFrame *const receiveFrameBuffer,
                          const unsigned char const receiveFrameBufferLength) {
    LRP_initFrameBuffer(receiveFrameBuffer, &receiveFrameBufferLength);

    receiveLayer->receiveDeviceId = receiveDeviceId;
    receiveLayer->frameBuffer = receiveFrameBuffer;
    receiveLayer->handlerCurrentFrame =
    receiveLayer->controllerCurrentFrame = &receiveLayer->frameBuffer[0];
    LRP_resetReceiveLayer(receiveLayer);

    receiveLayer->framingErrorHandler =
    receiveLayer->overrunErrorHandler =
    receiveLayer->parityBitErrorHandler = LRP_noReceiveErrorCallBack;
}

void LRP_receiveLayerController(_LRPReceiveLayer *const receiveLayer,
                                _LRPReceiveFrameController *const receiveFrameControllerList,
                                const unsigned char const receiveFrameControllerListLength) {
    if (receiveLayer->controllerCurrentFrame->status != RECEIVE_FRAME_COMPLETED) {
        return;
    }

    for (unsigned char i = 0; i < receiveFrameControllerListLength; i++) {
        if (receiveFrameControllerList[i](&receiveLayer->controllerCurrentFrame->sourceDeviceId,
                                          receiveLayer->controllerCurrentFrame->data)) {
            break;
        }
    }

    LRP_resetFrameStatus(receiveLayer->controllerCurrentFrame);
    receiveLayer->controllerCurrentFrame = receiveLayer->controllerCurrentFrame->next;
}

void
LRP_receiveLayerHandler(_LRPReceiveLayer *const receiveLayer, unsigned char data,
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
            LRP_readTargetDeviceIdAndCommandFromHeader1Data(receiveLayer->handlerCurrentFrame, &data);
            break;
        case 2:
            LRP_readSourceDeviceIdFromHeader2Data(receiveLayer->handlerCurrentFrame, &data);
            break;
        default:
            receiveLayer->handlerCurrentFrame->data[receiveLayer->numberOfReadBytes - 3] = data;
    }

    if (receiveLayer->handlerCurrentFrame->targetDeviceId != *receiveLayer->receiveDeviceId &&
        receiveLayer->handlerCurrentFrame->targetDeviceId != FRAME_BROADCAST_ID) {
        LRP_setReceiveLayerError(receiveLayer);
    }

    if (LRP_isReadLastFrameByte(&receiveLayer->numberOfReadBytes)) {
        if (receiveLayer->status == RECEIVE_LAYER_STATUS_OK) {
            receiveLayer->handlerCurrentFrame->status = RECEIVE_FRAME_COMPLETED;

        }
        LRP_prepareReceiveLayerToTheNextIteration(receiveLayer);
    }
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

void LRP_noReceiveErrorCallBack(void) {}

void LRP_setReceiveLayerError(_LRPReceiveLayer *const receiveLayer) {
    receiveLayer->status = RECEIVE_LAYER_STATUS_ERROR;
    LRP_resetFrameStatus(receiveLayer->handlerCurrentFrame);
}

void LRP_prepareReceiveLayerToTheNextIteration(_LRPReceiveLayer *const receiveLayer) {
    if (receiveLayer->handlerCurrentFrame->status == FRAME_READY_TO_REDEFINE) {
        LRP_resetReceiveLayer(receiveLayer);
        return;
    }

    if (receiveLayer->handlerCurrentFrame->next->status == FRAME_READY_TO_REDEFINE) {
        receiveLayer->handlerCurrentFrame = receiveLayer->handlerCurrentFrame->next;
        LRP_resetReceiveLayer(receiveLayer);
        return;
    }

    receiveLayer->status = RECEIVE_LAYER_STATUS_ERROR;
}

unsigned char LRP_isReadLastFrameByte(const unsigned char *const numberOfReceivedBytes) {
    if (*numberOfReceivedBytes == FRAME_LENGTH) {
        return 1;
    }
    return 0;
}

void LRP_resetReceiveLayer(_LRPReceiveLayer *const receiveLayer) {
    receiveLayer->numberOfReadBytes = 0;
    receiveLayer->status = RECEIVE_LAYER_STATUS_OK;
    receiveLayer->handlerCurrentFrame->status = FRAME_IN_PROGRESS;
}