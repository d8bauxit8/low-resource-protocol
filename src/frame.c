#include "frame.h"

void LRP_initFrameBuffer(_LRPFrame *const frameBuffer, const unsigned char *const frameBufferLength) {
    unsigned char i = 0;
    LRP_resetFrameStatus(&frameBuffer[i++]);
    for (; i < *frameBufferLength; i++) {
        LRP_resetFrameStatus(&frameBuffer[i]);
        frameBuffer[i - 1].next = &frameBuffer[i];
    }
    frameBuffer[i - 1].next = &frameBuffer[0];
}

void LRP_resetFrameStatus(_LRPFrame *const receiveFrame) {
    receiveFrame->status = FRAME_READY_TO_REDEFINE;
}

unsigned char LRP_createParityBit(unsigned char data) {
    unsigned char parity = 1;

    while (data) {
        parity ^= data & 1;
        data >>= 1;
    }
    return parity;
}

unsigned char LRP_isInvalidParityBit(unsigned char data, const unsigned char *const parityBit) {
    data = data ^ data >> 4;
    data = data ^ data >> 2;
    data = data ^ data >> 1;
    return (data & 1) == *parityBit;
}

void LRP_readTargetDeviceIdAndCommandFromHeader1Data(_LRPFrame *const frame, const unsigned char *const data) {
    frame->targetDeviceId = (*data & FRAME_TARGET_DEVICE_ID_MASK) >> 3;
    frame->command = (*data & FRAME_COMMAND_MASK) >> 1;
}

void LRP_readSourceDeviceIdFromHeader2Data(_LRPFrame *const frame, const unsigned char *const data) {
    frame->sourceDeviceId = (*data & FRAME_SOURCE_DEVICE_ID_MASK) >> 3;
}