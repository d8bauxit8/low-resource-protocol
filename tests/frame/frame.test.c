#include "frame.test.h"

_LRPFrame *
TEST_createLRPFrameInWhichTheBufferIsFilled(const unsigned char const targetDeviceId, const unsigned char const command,
                                            const unsigned char const sourceDeviceId,
                                            const unsigned char const length) {
    static _LRPFrame frame;
    frame.buffer[0] = (targetDeviceId << 3) | command;
    frame.buffer[1] = (sourceDeviceId << 3) | length;
    return &frame;
}