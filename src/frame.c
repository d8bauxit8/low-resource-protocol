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

unsigned char LRP_getTargetDeviceIdFromReceivedByte(const unsigned char *const data) {
    return (*data & FRAME_TARGET_DEVICE_ID_MASK) >> 3;
}

void LRP_addHeaderDataToFrameDataFromFrameBuffer(_LRPFrame *const frame) {
    frame->targetDeviceId = LRP_getTargetDeviceIdFromReceivedByte(&frame->buffer[0]);
    frame->command = frame->buffer[0] & FRAME_COMMAND_MASK;
    frame->sourceDeviceId = (frame->buffer[1] & FRAME_SOURCE_DEVICE_ID_MASK) >> 3;
    frame->length = frame->buffer[1] & FRAME_LENGTH_MASK;
}

void LRP_addDataToFrameDataFromFrameBuffer(_LRPFrame *const frame) {
    for (unsigned char i = 0; i < frame->length; i++) {
        frame->data[i] = &frame->buffer[i + FRAME_NUMBER_OF_HEADER_BYTES];
    }
}

void LRP_addHeaderDataToFrameBufferFromFrameData(_LRPFrame *const frame) {
    frame->buffer[0] = (frame->targetDeviceId << 3) | (FRAME_COMMAND_MASK & frame->command);
    frame->buffer[1] = (frame->sourceDeviceId << 3) | (FRAME_LENGTH_MASK & frame->length);
}

void LRP_addDataToFrameBufferFromFrameData(_LRPFrame *const frame) {
    for (unsigned char i = 0; i < frame->length; i++) {
        frame->buffer[i + FRAME_NUMBER_OF_HEADER_BYTES] = *frame->data[i];
    }
}