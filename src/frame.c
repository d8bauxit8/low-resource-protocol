#include "frame.h"

void LRP_initFrame(_LRPFrame *const frameBuffer, const unsigned char *const frameBufferLength) {
    for (char i = 0; i < *frameBufferLength; i++) {
        LRP_resetFrame(&frameBuffer[i]);
    }
}

void LRP_resetFrame(_LRPFrame *const receiveFrame) {
    receiveFrame->status = FRAME_READY_TO_REDEFINE;
}

char LRP_findFirstFrameIndexByStatus(_LRPFrame *const frameBuffer, const unsigned char *const frameBufferLength,
                                     const unsigned char status) {
    for (char i = 0; i < *frameBufferLength; i++) {
        if (frameBuffer[i].status == status) {
            return i;
        }
    }
    return -1;
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