//
// Created by Jehoda Márk on 2022. 06. 01..
//

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMIT_SESSION_PROVIDER_H
#define LOW_RESOURCE_PROTOCOL_TRANSMIT_SESSION_PROVIDER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "transmit-structures.h"

void LRP_TransmitSessionProvider_init(LRPTransmitSessionProvider *sessionProvider,
                                     const unsigned char *deviceId,
                                     LRPFrame *frameBuffer,
                                     unsigned char frameBufferLength);

#ifdef    __cplusplus
}
#endif

#endif // LOW_RESOURCE_PROTOCOL_TRANSMIT_SESSION_PROVIDER_H
