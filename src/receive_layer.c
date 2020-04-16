#include "receive_layer.h"

void LRP_setReceiveLayerError(_LRPReceiveLayer *const receiveLayer);

void LRP_prepareReceiveLayerToTheNextIteration(_LRPReceiveLayer *const receiveLayer);

void LRP_setReceiveLayerOK(_LRPReceiveLayer *const receiveLayer);

void LRP_initReceiveLayer(_LRPReceiveLayer *const receiveLayer, const unsigned char *const receiveDeviceId,
                          _LRPFrame *const receiveFrameBuffer,
                          const unsigned char const receiveFrameBufferLength) {
    LRP_initFrameBuffer(receiveFrameBuffer, &receiveFrameBufferLength);

    receiveLayer->receiveDeviceId = receiveDeviceId;

    receiveLayer->frameBuffer = receiveFrameBuffer;

    receiveLayer->handlerCurrentFrame =
    receiveLayer->controllerCurrentFrame = &receiveLayer->frameBuffer[0];

    receiveLayer->numberOfReadBytes = 0;

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
    if (!receiveLayer->numberOfReadBytes) {
        LRP_prepareReceiveLayerToTheNextIteration(receiveLayer);
    }

    receiveLayer->numberOfReadBytes++;

    if (receiveLayer->status != RECEIVE_LAYER_STATUS_SKIP) {
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
                if (receiveLayer->handlerCurrentFrame->targetDeviceId != *receiveLayer->receiveDeviceId &&
                    receiveLayer->handlerCurrentFrame->targetDeviceId != FRAME_BROADCAST_ID) {
                    LRP_setReceiveLayerError(receiveLayer);
                }
                break;
            case 2:
                LRP_readSourceDeviceIdFromHeader2Data(receiveLayer->handlerCurrentFrame, &data);
                break;
            default:
                receiveLayer->handlerCurrentFrame->data[receiveLayer->numberOfReadBytes - 3] = data;
        }

        if (receiveLayer->numberOfReadBytes == FRAME_LENGTH) {
            if (receiveLayer->status == RECEIVE_LAYER_STATUS_OK) {
                receiveLayer->handlerCurrentFrame->status = RECEIVE_FRAME_COMPLETED;
                receiveLayer->handlerCurrentFrame = receiveLayer->handlerCurrentFrame->next;
            }
        }
    }

    if (receiveLayer->numberOfReadBytes == FRAME_LENGTH) {
        receiveLayer->numberOfReadBytes = 0;
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
    receiveLayer->status = RECEIVE_LAYER_STATUS_SKIP;
    LRP_resetFrameStatus(receiveLayer->handlerCurrentFrame);
}

void LRP_prepareReceiveLayerToTheNextIteration(_LRPReceiveLayer *const receiveLayer) {
    if (receiveLayer->handlerCurrentFrame->status == RECEIVE_FRAME_COMPLETED) {
        receiveLayer->status = RECEIVE_LAYER_STATUS_SKIP;
        return;
    }
    LRP_setReceiveLayerOK(receiveLayer);
}

void LRP_setReceiveLayerOK(_LRPReceiveLayer *const receiveLayer) {
    receiveLayer->status = RECEIVE_LAYER_STATUS_OK;
    receiveLayer->handlerCurrentFrame->status = FRAME_IN_PROGRESS;
}