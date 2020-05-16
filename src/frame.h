/* 
 * File:   frame.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef FRAME_H
#define    FRAME_H

#ifdef    __cplusplus
extern "C" {
#endif

#define FRAME_NUMBER_OF_MAX_DATA_BYTES 7
#define FRAME_NUMBER_OF_HEADER_BYTES 2
#define FRAME_NUMBER_OF_FRAME_BYTES FRAME_NUMBER_OF_HEADER_BYTES + FRAME_NUMBER_OF_MAX_DATA_BYTES

typedef struct _FrameData {
    unsigned char sourceDeviceId;
    unsigned char targetDeviceId;
    unsigned char command;
    unsigned char length;
    unsigned char *data[FRAME_NUMBER_OF_MAX_DATA_BYTES];
} _FrameData;

typedef struct _LRPFrame {
    struct _FrameData;
    unsigned char status;
    unsigned char buffer[FRAME_NUMBER_OF_FRAME_BYTES];
    struct _LRPFrame *next;
} _LRPFrame;

// Statuses
#define FRAME_READY_TO_REDEFINE 0

#define FRAME_BROADCAST_ID 0b00011111

void LRP_Frame_initBuffer(_LRPFrame *const frameBuffer, const unsigned char *const frameBufferLength);

void LRP_Frame_setStatus(_LRPFrame *const frame, const unsigned char const status);

void LRP_Frame_resetStatus(_LRPFrame *const frame);

unsigned char LRP_Frame_getTargetDeviceIdFromReceivedByte(const unsigned char *const data);

void LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(_LRPFrame *const frame);

void LRP_Frame_addDataToFrameDataFromFrameBuffer(_LRPFrame *const frame);

void LRP_Frame_addHeaderDataToFrameBufferFromFrameData(_LRPFrame *const frame);

void LRP_Frame_addDataToFrameBufferFromFrameData(_LRPFrame *const frame);

#ifdef    __cplusplus
}
#endif

#endif    /* FRAME_H */

