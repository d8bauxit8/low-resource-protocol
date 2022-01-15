#include "receive-line-code-layer.h"

/**
 * Public method declarations
 */
void LRP_ReceiveLineCodeLayer_handler(LRPReceiveSessionProvider *const sessionProvider,
                                      LRPLineCode4B5B *const lineCode4B5B,
                                      const unsigned char *const data) {
    if (*data == LINE_CODE_4B5B_START_DELIMITER_BYTE && LRP_ReceiveLinkLayer_isStartReceiving(sessionProvider)) {
        LRP_4B5B_reset(lineCode4B5B);
        return;
    }

    if (LRP_LinkLayer_isStatusOK((LRPSessionProvider *) sessionProvider)) {
        if (*data == LINE_CODE_4B5B_STOP_DELIMITER_BYTE) {
            LRP_ReceiveLinkLayer_stopReceiving(sessionProvider);
            return;
        }

        LRP_4B5B_addEncodedByteToBufferOfEncodedBits(lineCode4B5B, data);

        if (LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(lineCode4B5B)) {
            // Try to read a decoded byte
            unsigned char decodingState;
            const unsigned char decodedData = LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(
                    lineCode4B5B, &decodingState);

            // If the received byte could not be decoded,
            // the frame should be threw because it would be invalid.
            if (LRP_4B5B_isDecodingFailed(&decodingState)) {
                LRP_LinkLayer_setError((LRPSessionProvider *) sessionProvider, LINK_LAYER_DECODE_ERROR);
                return;
            }

            // If everything is OK, the decoded byte can be read.
            LRP_ReceiveLinkLayer_handler(sessionProvider, &decodedData);
        }
    }
}