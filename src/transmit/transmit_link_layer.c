#include "transmit_link_layer.h"

/**
 * Public method declarations
 */
unsigned char
LRP_TransmitLinkLayer_handler(LRPTransmitSessionProvider *const sessionProvider) {
    return sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfWroteBytes++];
}

unsigned char LRP_TransmitLinkLayer_isUnwrittenDataInBuffer(LRPTransmitSessionProvider *const sessionProvider) {
    return sessionProvider->indexOfWroteBytes <
           (FRAME_NUMBER_OF_HEADER_BYTES + sessionProvider->linkCurrentFrame->length);
}

unsigned char LRP_TransmitLinkLayer_isReadyToTransmit(LRPTransmitSessionProvider *const sessionProvider) {
    return sessionProvider->linkCurrentFrame->status == TRANSMIT_FRAME_READY_TO_TRANSMIT;
}

void LRP_TransmitLinkLayer_startTransmitting(LRPTransmitSessionProvider *const sessionProvider) {
    sessionProvider->indexOfWroteBytes = 0;
    LRP_LinkLayer_setOk((LRPSessionProvider *) sessionProvider);
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, TRANSMIT_FRAME_TRANSMITTING);
}

void LRP_TransmitLinkLayer_endTransmitting(LRPTransmitSessionProvider *const sessionProvider) {
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, FRAME_READY_TO_REDEFINE);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
    LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
}

void LRP_TransmitLinkLayer_errorStatusHandler(LRPTransmitSessionProvider *const sessionProvider) {
    if (LRP_LinkLayer_isStatusError((LRPSessionProvider *) sessionProvider)) {
        if (sessionProvider->linkCurrentFrame->status ==
            TRANSMIT_FRAME_TRANSMITTING) {
            LRP_Frame_setStatus(sessionProvider->linkCurrentFrame,
                                TRANSMIT_FRAME_READY_TO_TRANSMIT);
        }
        LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
    }
}