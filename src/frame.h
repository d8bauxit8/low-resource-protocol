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

#include "structures.h"

// Statuses 0-1 bits
// Other bits is not used
#define FRAME_READY_TO_REDEFINE 0b00000000
#define FRAME_IN_PROGRESS 0b00000011

#define FRAME_BROADCAST_ID 0b00011111

#define FRAME_SOURCE_DEVICE_ID_MASK 0b11111000
#define FRAME_COMMAND_MASK 0b00000110
#define FRAME_TARGET_DEVICE_ID_MASK FRAME_SOURCE_DEVICE_ID_MASK

void LRP_initFrameBuffer(_LRPFrame *const frameBuffer, const unsigned char *const frameBufferLength);

void LRP_resetFrame(_LRPFrame *const receiveFrame);

unsigned char LRP_createParityBit(unsigned char data);

unsigned char LRP_isInvalidParityBit(unsigned char data, const unsigned char *const parityBit);

void LRP_readTargetDeviceIdAndCommandFromHeader1Data(_LRPFrame *const frame, const unsigned char *const data);

void LRP_readSourceDeviceIdFromHeader2Data(_LRPFrame *const frame, const unsigned char *const data);

#ifdef    __cplusplus
}
#endif

#endif    /* FRAME_H */

