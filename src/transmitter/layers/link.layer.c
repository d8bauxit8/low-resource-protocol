#include "link.layer.h"

/**
 * Public method declarations
 */
unsigned char
LRP_TransmitterLinkLayer_handler(LRPTransmitterSessionProvider *const sessionProvider) {
    return sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfWrittenBytes++];
}

unsigned char
LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(const LRPTransmitterSessionProvider *const sessionProvider) {
    return sessionProvider->indexOfWrittenBytes <
           (FRAME_NUMBER_OF_HEADER_BYTES + sessionProvider->linkCurrentFrame->length);
}

unsigned char
LRP_TransmitterLinkLayer_isReadyToTransmitter(const LRPTransmitterSessionProvider *const sessionProvider) {
    return sessionProvider->linkCurrentFrame->status == TRANSMITTER_FRAME_READY_TO_TRANSMITTER;
}

void LRP_TransmitterLinkLayer_startTransmitting(LRPTransmitterSessionProvider *const sessionProvider) {
    sessionProvider->indexOfWrittenBytes = 0;
    LRP_LinkLayer_setOk((LRPSessionProvider *) sessionProvider);
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, TRANSMITTER_FRAME_TRANSMITTERTING);
}

void LRP_TransmitterLinkLayer_endTransmitting(LRPTransmitterSessionProvider *const sessionProvider) {
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, FRAME_READY_TO_REDEFINE);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
    LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
}

void LRP_TransmitterLinkLayer_errorStatusHandler(LRPTransmitterSessionProvider *const sessionProvider) {
    if (LRP_LinkLayer_isStatusError((LRPSessionProvider *) sessionProvider)) {
        if (sessionProvider->linkCurrentFrame->status ==
            TRANSMITTER_FRAME_TRANSMITTERTING) {
            LRP_Frame_setStatus(sessionProvider->linkCurrentFrame,
                                TRANSMITTER_FRAME_READY_TO_TRANSMITTER);
        }
        LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
    }
}