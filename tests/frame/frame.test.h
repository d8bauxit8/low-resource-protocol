/*
 * File:   frame.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef FRAME_TEST_H
#define    FRAME_TEST_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../../src/frame.h"

_LRPFrame *
TEST_createLRPFrameInWhichTheBufferIsFilled(const unsigned char const targetDeviceId, const unsigned char const command,
                                            const unsigned char const sourceDeviceId, const unsigned char const length);

#ifdef    __cplusplus
}
#endif

#endif    /* FRAME_TEST_H */

