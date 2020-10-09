#include "transmit_line_code_layer.h"

/**
 * Public method declarations
 */
void
LRP_TransmitLineCodeLayer_handler(_LRPTransmitSessionProvider *const sessionProvider,
                                  _LRPLineCode4B5B *const lineCode4B5B, unsigned char *const data) {
    if (LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(sessionProvider)) {
        LRP_TransmitLineCodeLayer_startTransmitting(sessionProvider, lineCode4B5B,
                                                    data);
        return;
    }

    if (LRP_LinkLayer_isStatusOK((_LRPSessionProvider *) sessionProvider)) {
        if (LRP_TransmitLinkLayer_isEndOfBufferLength(sessionProvider)) {

            if (LRP_4B5B_isBufferOfEncodedBitsReadyToReadAnEncodedByte(lineCode4B5B)) {
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

            LRP_TransmitLinkLayer_endTransmitting((_LRPSessionProvider *) sessionProvider);
            *data = LINE_CODE_4B5B_END_DELIMITER_BYTE;
        }
    }
}

void LRP_TransmitLineCodeLayer_startTransmitting(
        _LRPTransmitSessionProvider *const sessionProvider,
        _LRPLineCode4B5B *const lineCode4B5B, unsigned char *const data) {

    LRP_TransmitLinkLayer_startTransmitting(sessionProvider);
    LRP_4B5B_reset(lineCode4B5B);
    *data = LINE_CODE_4B5B_START_DELIMITER_BYTE;
}

unsigned char LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(
        _LRPTransmitSessionProvider *const sessionProvider) {
    if (LRP_TransmitLinkLayer_isReadyToTransmit(sessionProvider) && !LRP_LinkLayer_isStatusError(
            (_LRPSessionProvider *) sessionProvider)) {
        return 1;
    }
    return 0;
}