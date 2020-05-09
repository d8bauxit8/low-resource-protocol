/* 
 * File:   layers.h
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

// Statuses 0-1 bits
// Other bits is not used
#define FRAME_READY_TO_REDEFINE 0b00000000

#define FRAME_BROADCAST_ID 0b00011111

void LRP_initFrameBuffer(_LRPFrame *const frameBuffer, const unsigned char *const frameBufferLength);

void LRP_setFrameStatus(_LRPFrame *const frame, const unsigned char const status);

void LRP_resetFrameStatus(_LRPFrame *const frame);

void LRP_readTargetDeviceIdAndCommandFromReceivedByte(_LRPFrame *const frame, const unsigned char *const data);

void LRP_readSourceDeviceIdAndDataLengthFromReceivedByte(_LRPFrame *const frame, const unsigned char *const data);

void LRP_readDataFromBuffer(_LRPFrame *const frame);

#ifdef    __cplusplus
}
#endif

#endif    /* FRAME_H */

