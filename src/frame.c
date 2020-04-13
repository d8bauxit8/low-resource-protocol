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

unsigned char LRP_isInvalidParityBit(unsigned char data, const unsigned char *const parityBit) {
    data = data ^ data >> 4;
    data = data ^ data >> 2;
    data = data ^ data >> 1;
    return (data & 1) == *parityBit;
}