#include "receive_frame.h"

void LRP_readTargetDeviceIdAndCommandFromHeader1Data(_LRPFrame *const frame, const unsigned char *const data);

void LRP_readSourceDeviceIdFromHeader2Data(_LRPFrame *const frame, const unsigned char *const data);

void LRP_receiveFrameController(_LRPFrame *const receiveFrame, const unsigned char *const numberOfReadBytes,
                                unsigned char data) {
    switch (*numberOfReadBytes) {
        case 1:
            LRP_readTargetDeviceIdAndCommandFromHeader1Data(receiveFrame, &data);
            break;
        case 2:
            LRP_readSourceDeviceIdFromHeader2Data(receiveFrame, &data);
            break;
        default:
            receiveFrame->data[*numberOfReadBytes - 3] = data;
    }
}

void LRP_readTargetDeviceIdAndCommandFromHeader1Data(_LRPFrame *const receiveFrame, const unsigned char *const data) {
    receiveFrame->targetDeviceId = (*data & FRAME_TARGET_DEVICE_ID_MASK) >> 3;
    receiveFrame->command = (*data & FRAME_COMMAND_MASK) >> 1;
}

void LRP_readSourceDeviceIdFromHeader2Data(_LRPFrame *const receiveFrame, const unsigned char *const data) {
    receiveFrame->sourceDeviceId = (*data & FRAME_SOURCE_DEVICE_ID_MASK) >> 3;
}
