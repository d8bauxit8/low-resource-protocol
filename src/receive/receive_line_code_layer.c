#include "receive_line_code_layer.h"

unsigned char LRP_ReceiveLineCodeLayer_isReadyToStartReceiving(
        _LRPReceiveSessionProvider *sessionProvider, const unsigned char *data);

/**
 * Public method declarations
 */
void LRP_ReceiveLineCodeLayer_handler(_LRPReceiveSessionProvider *const sessionProvider,
                                      _LRPLineCode4B5B *const lineCode4B5B,
                                      const unsigned char *const data) {
    if (LRP_ReceiveLineCodeLayer_isReadyToStartReceiving(sessionProvider, data)) {
        LRP_ReceiveLinkLayer_startReceiving(sessionProvider);
        LRP_4B5B_reset(lineCode4B5B);
        return;
    }

    if (LRP_LinkLayer_isStatusOK((_LRPSessionProvider *) sessionProvider)) {
        if (*data == LINE_CODE_4B5B_END_DELIMITER_BYTE) {
            LRP_ReceiveLinkLayer_endReceiving((_LRPSessionProvider *) sessionProvider);
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
                LRP_LinkLayer_setError((_LRPSessionProvider *) sessionProvider);
                return;
            }

            // If everything is OK, the decoded byte can be read.
            LRP_ReceiveLinkLayer_handler(sessionProvider, &decodedData);
        }
    }
}

unsigned char LRP_ReceiveLineCodeLayer_isReadyToStartReceiving(
        _LRPReceiveSessionProvider *const sessionProvider, const unsigned char *const data) {
    if (*data == LINE_CODE_4B5B_START_DELIMITER_BYTE) {
        if (!LRP_LinkLayer_isStatusError(
                (_LRPSessionProvider *) sessionProvider)) {
            return 1;
        }
    }
    return 0;
}