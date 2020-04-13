/* 
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef RECEIVE_FRAME_H
#define    RECEIVE_FRAME_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "structures.h"
#include "frame.h"
// Status flow
// 1. In progress
// 2. Completed
// 3. Ready to redefine
#define RECEIVE_FRAME_COMPLETED 0b00000010

void LRP_receiveFrameController(_LRPFrame *const receiveFrame, const unsigned char *const numberOfReadBytes,
                                unsigned char data);

#ifdef    __cplusplus
}
#endif

#endif    /* RECEIVE_FRAME_H */

