#include "transmit_line_code_layer.h"

/**
 * Public method declarations
 */
unsigned char
LRP_TransmitLineCodeLayer_handler(_LRPTransmitSessionProvider *const sessionProvider,
                                  _LRPLineCode4B5B *const lineCode4B5B) {
    if (LRP_TransmitLinkLayer_isReadyToTransmit(sessionProvider)) {
        LRP_TransmitLinkLayer_startTransmitting(sessionProvider);
        LRP_4B5B_reset(lineCode4B5B);
        return START_DELIMITER_BYTE_4B5B;
    }

    if (LRP_LinkLayer_isStatusOK((_LRPSessionProvider *) sessionProvider)) {
        if (LRP_TransmitLinkLayer_isEndOfBufferLength(sessionProvider)) {

            if (LRP_4B5B_isBufferOfEncodedBitsReadyToReadAnEncodedByte(lineCode4B5B)) {
                const unsigned char const data = LRP_TransmitLinkLayer_handler(sessionProvider);
                LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(lineCode4B5B,
                                                                     &data);
            }

            return LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(lineCode4B5B);
        } else {
            if (LRP_4B5B_isThereRemainingBitsInBufferOfEncodedBits(lineCode4B5B)) {
                return LRP_4B5B_readAnRemainingBitsFromBufferOfEncodedBits(lineCode4B5B);
            }

            LRP_TransmitLinkLayer_endTransmitting((_LRPSessionProvider *) sessionProvider);
            return END_DELIMITER_BYTE_4B5B;
        }
    }
    return 0;
}