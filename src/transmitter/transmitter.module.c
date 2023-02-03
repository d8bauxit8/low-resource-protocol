//
// Created by Jehoda Márk on 2022. 06. 01..
//

#include "transmitter.module.h"

void
LRP_Transmitter_init(LRPTransmitterSessionProvider *sessionProvider,
                     unsigned char *const deviceId,
                     LRPFrame *frameBuffer,
                     unsigned char frameBufferLength) {
    LRP_SessionProvider_init((LRPSessionProvider *) sessionProvider, deviceId, frameBuffer, frameBufferLength);
}
