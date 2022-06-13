//
// Created by Jehoda Márk on 2022. 06. 01..
//

#include "receive-session-provider.h"

void
LRP_ReceiveSessionProvider_init(LRPReceiveSessionProvider *const sessionProvider,
                                const unsigned char *const deviceId,
                                LRPFrame *const frameBuffer,
                                const unsigned char frameBufferLength,
                                const unsigned char *const groupId) {
    LRP_SessionProvider_init((LRPSessionProvider *) sessionProvider, deviceId, frameBuffer, frameBufferLength);
    sessionProvider->groupId = (unsigned char *) groupId;
}
