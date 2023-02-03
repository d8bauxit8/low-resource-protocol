//
// Created by Jehoda Márk on 2023. 02. 03..
//

#ifndef LOW_RESOURCE_PROTOCOL_RECEIVER_FRAME_CONTROLLER_DATA_H
#define LOW_RESOURCE_PROTOCOL_RECEIVER_FRAME_CONTROLLER_DATA_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../../frame.h"

typedef unsigned char (*LRPReceiverFrameController)(const LRPFrameData *const frameData);

#ifdef    __cplusplus
}
#endif

#endif //LOW_RESOURCE_PROTOCOL_RECEIVER_FRAME_CONTROLLER_DATA_H
