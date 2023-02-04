#include "line-code.layer.h"

/**
 * Public method declarations
 */
void LRP_ReceiverLineCodeLayer_handler(LRPReceiverSessionProvider *const sessionProvider,
                                       LRPLineCode4B5B *const lineCode4B5B,
                                       const unsigned char *const data) {
    if (*data == LRP_LINE_CODE_4B5B_START_DELIMITER_BYTE && LRP_ReceiverLinkLayer_isStartReceiving(sessionProvider)) {
        LRP_ReceiverLinkLayer_startReceiving(sessionProvider);
        LRP_4B5B_reset(lineCode4B5B);
        return;
    }

    if (LRP_LinkLayer_isStatusOK((LRPSessionProvider *) sessionProvider)) {
        if (*data == LRP_LINE_CODE_4B5B_STOP_DELIMITER_BYTE) {
            LRP_ReceiverLinkLayer_stopReceiving(sessionProvider);
            return;
        }

        LRP_4B5B_addEncodedByteToBufferOfEncodedBits(lineCode4B5B, data);

        if (LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(lineCode4B5B)) {
            // Try to read a decoded byte
            LRPLineCode4B5BDecodingState decodingState;
            const unsigned char decodedData = LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(
                    lineCode4B5B, &decodingState);

            // If the received byte could not be decoded,
            // the frame should be thrown because it would be invalid.
            if (LRP_4B5B_isDecodingFailed(&decodingState)) {
                LRP_LinkLayer_setError((LRPSessionProvider *) sessionProvider, LRPLinkLayerErrorCode_DecodeError);
                return;
            }

            // If everything is OK, the decoded byte can be read.
            LRP_ReceiverLinkLayer_handler(sessionProvider, &decodedData);
        }
    }
}