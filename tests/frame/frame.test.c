#include "frame.test.h"

_LRPFrame globFrame;

_LRPFrame *
TEST_createLRPFrameInWhichTheBufferIsFilled(const unsigned char const targetDeviceId, const unsigned char const command,
                                            const unsigned char const sourceDeviceId,
                                            const unsigned char const length) {
    _LRPFrame frameTmp;
    frameTmp.status = 0xFF;
    frameTmp.buffer[0] = (targetDeviceId << 3) | command;
    frameTmp.buffer[1] = (sourceDeviceId << 3) | length;
    globFrame = frameTmp;
    return &globFrame;
}