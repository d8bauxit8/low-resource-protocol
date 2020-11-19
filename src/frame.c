#include "frame.h"

#define FRAME_TARGET_DEVICE_ID_MASK 0b11111000u
#define FRAME_COMMAND_MASK 0b00000111u
#define FRAME_SOURCE_DEVICE_ID_MASK 0b11111000u
#define FRAME_LENGTH_MASK 0b00000111u

/**
 * Public method declarations
 */
void LRP_Frame_initBuffer(LRPFrame *const frameBuffer, const unsigned char *const frameBufferLength) {
    unsigned char i = 0;
    LRP_Frame_resetStatus(&frameBuffer[i++]);
    for (; i < *frameBufferLength; i++) {
        LRP_Frame_resetStatus(&frameBuffer[i]);
        frameBuffer[i - 1].next = &frameBuffer[i];
    }
    frameBuffer[i - 1].next = &frameBuffer[0];
}

void LRP_Frame_setStatus(LRPFrame *const frame, const unsigned char status) {
    frame->status = status;
}

void LRP_Frame_resetStatus(LRPFrame *const frame) {
    frame->status = FRAME_READY_TO_REDEFINE;
}

unsigned char LRP_Frame_getTargetDeviceIdFromReceivedByte(const unsigned char *const data) {
    return (*data & FRAME_TARGET_DEVICE_ID_MASK) >> 3u;
}

void LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(LRPFrame *const frame) {
    frame->targetDeviceId = LRP_Frame_getTargetDeviceIdFromReceivedByte(&frame->buffer[0]);
    frame->command = frame->buffer[0] & FRAME_COMMAND_MASK;
    frame->sourceDeviceId = (frame->buffer[1] & FRAME_SOURCE_DEVICE_ID_MASK) >> 3u;
    frame->length = frame->buffer[1] & FRAME_LENGTH_MASK;
}

void LRP_Frame_addDataToFrameDataFromFrameBuffer(LRPFrame *const frame) {
    for (unsigned char i = 0; i < frame->length; i++) {
        frame->data[i] = &frame->buffer[i + FRAME_NUMBER_OF_HEADER_BYTES];
    }
}

void LRP_Frame_addHeaderDataToFrameBufferFromFrameData(LRPFrame *const frame) {
    frame->buffer[0] = (unsigned char) (frame->targetDeviceId << 3u) | (FRAME_COMMAND_MASK & frame->command);
    frame->buffer[1] = (unsigned char) (frame->sourceDeviceId << 3u) | (FRAME_LENGTH_MASK & frame->length);
}

void LRP_Frame_addDataToFrameBufferFromFrameData(LRPFrame *const frame) {
    for (unsigned char i = 0; i < frame->length; i++) {
        frame->buffer[i + FRAME_NUMBER_OF_HEADER_BYTES] = *frame->data[i];
    }
}