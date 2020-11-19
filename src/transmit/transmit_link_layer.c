#include "transmit_link_layer.h"

/**
 * Public method declarations
 */
unsigned char
LRP_TransmitLinkLayer_handler(_LRPTransmitSessionProvider *const sessionProvider) {
    return sessionProvider->linkCurrentFrame->buffer[sessionProvider->indexOfWroteBytes++];
}

unsigned char LRP_TransmitLinkLayer_isEndOfBufferLength(_LRPTransmitSessionProvider *const sessionProvider) {
    return sessionProvider->indexOfWroteBytes <
           (FRAME_NUMBER_OF_HEADER_BYTES + sessionProvider->linkCurrentFrame->length);
}

unsigned char LRP_TransmitLinkLayer_isReadyToTransmit(_LRPTransmitSessionProvider *const sessionProvider) {
    return sessionProvider->linkCurrentFrame->status == TRANSMIT_FRAME_READY_TO_TRANSMIT;
}

void LRP_TransmitLinkLayer_startTransmitting(_LRPTransmitSessionProvider *const sessionProvider) {
    sessionProvider->indexOfWroteBytes = 0;
    LRP_LinkLayer_setOk((LRPSessionProvider *) sessionProvider);
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, TRANSMIT_FRAME_TRANSMITTING);
}

void LRP_TransmitLinkLayer_endTransmitting(LRPSessionProvider *const sessionProvider) {
    LRP_Frame_setStatus(sessionProvider->linkCurrentFrame, FRAME_READY_TO_REDEFINE);
    sessionProvider->linkCurrentFrame = sessionProvider->linkCurrentFrame->next;
    LRP_LinkLayer_setSkip((LRPSessionProvider *) sessionProvider);
}

void LRP_TransmitLinkLayer_errorStatusHandler(LRPSessionProvider *const sessionProvider) {
    if (LRP_LinkLayer_isStatusError(sessionProvider)) {
        if (sessionProvider->linkCurrentFrame->status ==
            TRANSMIT_FRAME_TRANSMITTING) {
            LRP_Frame_setStatus(sessionProvider->linkCurrentFrame,
                                TRANSMIT_FRAME_READY_TO_TRANSMIT);
        }
        LRP_LinkLayer_setSkip(sessionProvider);
    }
}