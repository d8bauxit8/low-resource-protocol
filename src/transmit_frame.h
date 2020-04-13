/* 
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef TRANSMIT_FRAME_H
#define    TRANSMIT_FRAME_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "structures.h"
#include "frame.h"

// Status flow
// 1. Cached
// 2. In progress
// 3. Ready to redefine
#define RECEIVE_FRAME_COMPLETED 0b00000010

void LRP_transmitFrameController(_LRPFrame *const transmitFrame);

#ifdef    __cplusplus
}
#endif

#endif    /* TRANSMIT_FRAME_H */

