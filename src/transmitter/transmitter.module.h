//
// Created by Jehoda Márk on 2022. 06. 01..
//

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMITTER_MODULE_H
#define LOW_RESOURCE_PROTOCOL_TRANSMITTER_MODULE_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "data/session-provider.data.h"

void LRP_Transmitter_init(LRPTransmitterSessionProvider *sessionProvider,
                          unsigned char *deviceId,
                          LRPFrame *frameBuffer,
                          unsigned char frameBufferLength);

#ifdef    __cplusplus
}
#endif

#endif // LOW_RESOURCE_PROTOCOL_TRANSMITTER_MODULE_H
