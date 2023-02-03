//
// Created by Jehoda Márk on 2022. 06. 01..
//

#include "receiver.module.h"

void
LRP_Receiver_init(LRPReceiverSessionProvider *sessionProvider,
                  unsigned char *const deviceId,
                  LRPFrame *frameBuffer,
                  unsigned char frameBufferLength,
                  unsigned char *const groupId) {
    LRP_SessionProvider_init((LRPSessionProvider *) sessionProvider, deviceId, frameBuffer, frameBufferLength);
    sessionProvider->groupId = groupId;
}
