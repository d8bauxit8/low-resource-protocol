//
// Created by Jehoda Márk on 2022. 06. 01..
//

#include "transmit-session-provider.h"

void
LRP_TransmitSessionProvider_init(LRPTransmitSessionProvider *const sessionProvider,
                                const unsigned char *const deviceId,
                                LRPFrame *const frameBuffer,
                                const unsigned char frameBufferLength) {
    LRP_SessionProvider_init((LRPSessionProvider *) sessionProvider, deviceId, frameBuffer, frameBufferLength);
}
