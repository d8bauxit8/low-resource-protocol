/* 
 * File:   frame.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_FRAME_H
#define LOW_RESOURCE_PROTOCOL_FRAME_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "data/frame.data.h"

void LRP_Frame_initBuffer(LRPFrame *frameBuffer, const unsigned char *frameBufferLength);

void LRP_Frame_setStatus(LRPFrame *frame, unsigned char status);

void LRP_Frame_resetStatus(LRPFrame *frame);

unsigned char LRP_Frame_getTargetIdFromReceivedByte(const unsigned char *data);

unsigned char LRP_Frame_isGroupIdCommandFromReceivedByte(const unsigned char *data);

void LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(LRPFrame *frame);

void LRP_Frame_addDataToFrameDataFromFrameBuffer(LRPFrame *frame);

void LRP_Frame_addHeaderDataToFrameBufferFromFrameData(LRPFrame *frame);

void LRP_Frame_addDataToFrameBufferFromFrameData(LRPFrame *frame);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_FRAME_H */

