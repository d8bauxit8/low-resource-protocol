//
// Created by Jehoda Márk on 2022. 06. 01..
//

#ifndef LOW_RESOURCE_PROTOCOL_RECEIVE_SESSION_PROVIDER_H
#define LOW_RESOURCE_PROTOCOL_RECEIVE_SESSION_PROVIDER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "receive-structures.h"

void LRP_ReceiveSessionProvider_init(LRPReceiveSessionProvider *sessionProvider,
                                     const unsigned char *deviceId,
                                     LRPFrame *frameBuffer,
                                     unsigned char frameBufferLength,
                                     const unsigned char *groupId);

#ifdef    __cplusplus
}
#endif

#endif // LOW_RESOURCE_PROTOCOL_RECEIVE_SESSION_PROVIDER_H
