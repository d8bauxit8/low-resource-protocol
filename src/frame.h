/* 
 * File:   frame.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_FRAME_H
#define    LRP_FRAME_H

#ifdef    __cplusplus
extern "C" {
#endif

#define FRAME_NUMBER_OF_MAX_DATA_BYTES 7u
#define FRAME_NUMBER_OF_HEADER_BYTES 2u
#define FRAME_NUMBER_OF_FRAME_BYTES (FRAME_NUMBER_OF_HEADER_BYTES + FRAME_NUMBER_OF_MAX_DATA_BYTES)

typedef struct FrameData {
    unsigned char sourceDeviceId: 5;
    unsigned char targetDeviceId: 5;
    unsigned char command: 3;
    unsigned char length: 3;
    unsigned char *data[FRAME_NUMBER_OF_MAX_DATA_BYTES];
} FrameData;

typedef struct LRPFrame {
    unsigned char sourceDeviceId: 5;
    unsigned char targetDeviceId: 5;
    unsigned char command: 3;
    unsigned char length: 3;
    unsigned char *data[FRAME_NUMBER_OF_MAX_DATA_BYTES];
    unsigned char status;
    unsigned char buffer[FRAME_NUMBER_OF_FRAME_BYTES];
    struct LRPFrame *next;
} LRPFrame;

// Statuses
#define FRAME_READY_TO_REDEFINE 0u

#define FRAME_BROADCAST_ID 0b00011111u

void LRP_Frame_initBuffer(LRPFrame *frameBuffer, const unsigned char *frameBufferLength);

void LRP_Frame_setStatus(LRPFrame *frame, unsigned char status);

void LRP_Frame_resetStatus(LRPFrame *frame);

unsigned char LRP_Frame_getTargetDeviceIdFromReceivedByte(const unsigned char *data);

void LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(LRPFrame *frame);

void LRP_Frame_addDataToFrameDataFromFrameBuffer(LRPFrame *frame);

void LRP_Frame_addHeaderDataToFrameBufferFromFrameData(LRPFrame *frame);

void LRP_Frame_addDataToFrameBufferFromFrameData(LRPFrame *frame);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_FRAME_H */

