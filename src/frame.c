#include "frame.h"

#define FRAME_TARGET_DEVICE_ID_MASK 0b11111000
#define FRAME_COMMAND_MASK 0b00000111
#define FRAME_SOURCE_DEVICE_ID_MASK 0b11111000
#define FRAME_LENGTH_MASK 0b00000111

/**
 * Public method declarations
 */
void LRP_initFrameBuffer(_LRPFrame *const frameBuffer, const unsigned char *const frameBufferLength) {
    unsigned char i = 0;
    LRP_resetFrameStatus(&frameBuffer[i++]);
    for (; i < *frameBufferLength; i++) {
        LRP_resetFrameStatus(&frameBuffer[i]);
        frameBuffer[i - 1].next = &frameBuffer[i];
    }
    frameBuffer[i - 1].next = &frameBuffer[0];
}

void LRP_setFrameStatus(_LRPFrame *const frame, const unsigned char const status) {
    frame->status = status;
}

void LRP_resetFrameStatus(_LRPFrame *const frame) {
    frame->status = FRAME_READY_TO_REDEFINE;
}

void LRP_readTargetDeviceIdAndCommandFromReceivedByte(_LRPFrame *const frame, const unsigned char *const data) {
    frame->targetDeviceId = (*data & FRAME_TARGET_DEVICE_ID_MASK) >> 3;
    frame->command = *data & FRAME_COMMAND_MASK;
}

void LRP_readSourceDeviceIdAndDataLengthFromReceivedByte(_LRPFrame *const frame, const unsigned char *const data) {
    frame->sourceDeviceId = (*data & FRAME_SOURCE_DEVICE_ID_MASK) >> 3;
    frame->length = *data & FRAME_LENGTH_MASK;
}

void LRP_readDataFromBuffer(_LRPFrame *const frame) {
    for (unsigned char i = 0; i < frame->length; i++) {
        frame->data[i] = &frame->buffer[i + FRAME_NUMBER_OF_HEADER_BYTES];
    }
}