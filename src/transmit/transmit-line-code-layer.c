#include "transmit-line-code-layer.h"

/**
 * Public method declarations
 */
void
LRP_TransmitLineCodeLayer_handler(LRPTransmitSessionProvider *const sessionProvider,
                                  LRPLineCode4B5B *const lineCode4B5B, unsigned char *const data) {
    if (LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(sessionProvider)) {
        LRP_TransmitLineCodeLayer_startTransmitting(sessionProvider, lineCode4B5B,
                                                    data);
        return;
    }

    if (LRP_LinkLayer_isStatusOK((LRPSessionProvider *) sessionProvider)) {
        if (LRP_TransmitLinkLayer_isUnwrittenDataInBuffer(sessionProvider)) {

            if (LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(lineCode4B5B)) {
                const unsigned char bufferData = LRP_TransmitLinkLayer_handler(sessionProvider);
                LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(lineCode4B5B,
                                                                     &bufferData);
            }

            *data = LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(lineCode4B5B);
            return;
        } else {
            if (LRP_4B5B_isThereRemainingBitsInBufferOfEncodedBits(lineCode4B5B)) {
                *data = LRP_4B5B_readAnRemainingBitsFromBufferOfEncodedBits(lineCode4B5B);
                return;
            }

            LRP_TransmitLinkLayer_endTransmitting(sessionProvider);
            *data = LINE_CODE_4B5B_STOP_DELIMITER_BYTE;
        }
    }
}

void LRP_TransmitLineCodeLayer_startTransmitting(
        LRPTransmitSessionProvider *const sessionProvider,
        LRPLineCode4B5B *const lineCode4B5B, unsigned char *const data) {
    LRP_TransmitLinkLayer_startTransmitting(sessionProvider);
    LRP_4B5B_reset(lineCode4B5B);
    *data = LINE_CODE_4B5B_START_DELIMITER_BYTE;
}

unsigned char LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(
        const LRPTransmitSessionProvider *const sessionProvider) {
    if (LRP_TransmitLinkLayer_isReadyToTransmit(sessionProvider)) {
        if (!LRP_LinkLayer_isStatusError(
                (LRPSessionProvider *) sessionProvider)) {
            return 1;
        }
    }
    return 0;
}