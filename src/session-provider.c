#include "session-provider.h"

void LRP_SessionProvider_init(LRPSessionProvider *const sessionProvider, const unsigned char *const deviceId,
                              LRPFrame *const frameBuffer,
                              const unsigned char frameBufferLength) {
    LRP_Frame_initBuffer(frameBuffer, &frameBufferLength);

    sessionProvider->deviceId = (unsigned char *) deviceId;

    LRP_LinkLayer_setSkip(sessionProvider);

    sessionProvider->frameBuffer = frameBuffer;

    sessionProvider->linkCurrentFrame =
    sessionProvider->validatorCurrentFrame =
    sessionProvider->applicationCurrentFrame = &sessionProvider->frameBuffer[0];
}