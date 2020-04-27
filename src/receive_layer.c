#include "receive_layer.h"

void LRP_setReceiveLayerError(_LRPReceiveLayer *const receiveLayer);

unsigned char
LRP_isReceiveFrameSynchronizationStroke(const unsigned char *const data, const unsigned char *const parityBit);

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

void
LRP_receiveLayerHandler(_LRPReceiveLayer *const receiveLayer, unsigned char data,
                        const unsigned char *const parityBit,
                        const unsigned char *const framingError,
                        const unsigned char *const overrunError) {
    // If the number of read bytes' value is zero, it's prepared the receive layer to the next iteration.
    if (!receiveLayer->numberOfReadBytes) {
        LRP_prepareReceiveLayerToTheNextIteration(receiveLayer);
    }

    receiveLayer->numberOfReadBytes++;

    // If the receive layer status is not skip, we could read the data.
    if (receiveLayer->status != RECEIVE_LAYER_STATUS_SKIP) {
        // If there is framing error
        if (*framingError) {
            receiveLayer->framingErrorHandler();
            LRP_setReceiveLayerError(receiveLayer);
        }

        // If there is overrun error
        if (*overrunError) {
            receiveLayer->overrunErrorHandler();
            LRP_setReceiveLayerError(receiveLayer);
        }

        // If the received data's parity bit is invalid
        if (LRP_isInvalidParityBit(data, parityBit)) {
            // If it is a synchronization stroke, set the number of read bytes to zero.
            if (LRP_isReceiveFrameSynchronizationStroke(&data, parityBit)) {
                receiveLayer->numberOfReadBytes = 0;
                return;
            }
            receiveLayer->parityBitErrorHandler();
            LRP_setReceiveLayerError(receiveLayer);
        }

        switch (receiveLayer->numberOfReadBytes) {
            // It's the header 1
            case 1:
                LRP_readTargetDeviceIdAndCommandFromHeader1Data(receiveLayer->handlerCurrentFrame, &data);
                if (receiveLayer->handlerCurrentFrame->targetDeviceId != *receiveLayer->receiveDeviceId &&
                    receiveLayer->handlerCurrentFrame->targetDeviceId != FRAME_BROADCAST_ID) {
                    LRP_setReceiveLayerError(receiveLayer);
                }
                break;
                // Its the header 2
            case 2:
                LRP_readSourceDeviceIdFromHeader2Data(receiveLayer->handlerCurrentFrame, &data);
                break;
                // Its the data
            default:
                receiveLayer->handlerCurrentFrame->data[receiveLayer->numberOfReadBytes - 3] = data;
        }

        // If the number of read bytes reached the frame length
        if (receiveLayer->numberOfReadBytes == FRAME_LENGTH) {
            if (receiveLayer->status == RECEIVE_LAYER_STATUS_OK) {
                receiveLayer->handlerCurrentFrame->status = RECEIVE_FRAME_READY_TO_READ;
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

unsigned char
LRP_isReceiveFrameSynchronizationStroke(const unsigned char *const data, const unsigned char *const parityBit) {
    const unsigned char leftBits = *data & 0b11100000;
    const unsigned char rightBits = *data & 0b00011100;
    const unsigned char checkSum = ((*data & 0b00000011) << 1) | *parityBit;
    if ((leftBits ^ rightBits) == checkSum) {
        return 1;
    }
    return 0;
}

void LRP_prepareReceiveLayerToTheNextIteration(_LRPReceiveLayer *const receiveLayer) {
    if (receiveLayer->handlerCurrentFrame->status == RECEIVE_FRAME_READY_TO_READ) {
        receiveLayer->status = RECEIVE_LAYER_STATUS_SKIP;
        return;
    }
    LRP_setReceiveLayerOK(receiveLayer);
}

void LRP_setReceiveLayerOK(_LRPReceiveLayer *const receiveLayer) {
    receiveLayer->status = RECEIVE_LAYER_STATUS_OK;
    receiveLayer->handlerCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;
}