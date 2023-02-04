#include "line-code.layer.h"

/**
 * Public method declarations
 */
void
LRP_TransmitterLineCodeLayer_handler(LRPTransmitterSessionProvider *const sessionProvider,
                                     LRPLineCode4B5B *const lineCode4B5B, unsigned char *const data) {
    if (LRP_TransmitterLineCodeLayer_isReadyToStartTransmitting(sessionProvider)) {
        LRP_TransmitterLineCodeLayer_startTransmitting(sessionProvider, lineCode4B5B,
                                                       data);
        return;
    }

    if (LRP_LinkLayer_isStatusOK((LRPSessionProvider *) sessionProvider)) {
        if (LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(sessionProvider)) {

            if (LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(lineCode4B5B)) {
                const unsigned char bufferData = LRP_TransmitterLinkLayer_handler(sessionProvider);
                LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(lineCode4B5B,
                                                                     &bufferData);
            }

            *data = LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(lineCode4B5B);
            return;
        } else {
            if (LRP_4B5B_haveRemainingBitsInBufferOfEncodedBits(lineCode4B5B)) {
                *data = LRP_4B5B_readARemainingBitsFromBufferOfEncodedBits(lineCode4B5B);
                return;
            }

            LRP_TransmitterLinkLayer_endTransmitting(sessionProvider);
            *data = LRP_LINE_CODE_4B5B_STOP_DELIMITER_BYTE;
        }
    }
}

void LRP_TransmitterLineCodeLayer_startTransmitting(
        LRPTransmitterSessionProvider *const sessionProvider,
        LRPLineCode4B5B *const lineCode4B5B, unsigned char *const data) {
    LRP_TransmitterLinkLayer_startTransmitting(sessionProvider);
    LRP_4B5B_reset(lineCode4B5B);
    *data = LRP_LINE_CODE_4B5B_START_DELIMITER_BYTE;
}

unsigned char LRP_TransmitterLineCodeLayer_isReadyToStartTransmitting(
        const LRPTransmitterSessionProvider *const sessionProvider) {
    if (LRP_TransmitterLinkLayer_isReadyToTransmitter(sessionProvider)) {
        if (!LRP_LinkLayer_isStatusError(
                (LRPSessionProvider *) sessionProvider)) {
            return 1;
        }
    }
    return 0;
}