#include "session_provider.h"

void LRP_initSessionProvider(_LRPSessionProvider *const sessionProvider, const unsigned char *const deviceId,
                             _LRPFrame *const frameBuffer,
                             const unsigned char const frameBufferLength) {
    LRP_initFrameBuffer(frameBuffer, &frameBufferLength);

    sessionProvider->deviceId = deviceId;

    sessionProvider->frameBuffer = frameBuffer;

    sessionProvider->linkCurrentFrame =
    sessionProvider->validatorCurrentFrame =
    sessionProvider->applicationCurrentFrame = &sessionProvider->frameBuffer[0];

    sessionProvider->indexOfReadBytes = 0;
}