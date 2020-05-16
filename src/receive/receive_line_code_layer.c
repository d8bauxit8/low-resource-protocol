#include "receive_line_code_layer.h"

/**
 * Public method declarations
 */
void LRP_ReceiveLineCodeLayer_handler(_LRPReceiveSessionProvider *const sessionProvider,
                                      _LRPLineCode4B5B *const lineCode4B5B,
                                      const unsigned char *const data) {
    if (*data == START_DELIMITER_BYTE_4B5B) {
        LRP_ReceiveLinkLayer_startReceiving(sessionProvider);
        LRP_4B5B_reset(lineCode4B5B);
        return;
    }

    if (LRP_LinkLayer_isStatusOK((_LRPSessionProvider *) sessionProvider)) {
        if (*data == END_DELIMITER_BYTE_4B5B) {
            LRP_ReceiveLinkLayer_endReceiving((_LRPSessionProvider *) sessionProvider);
            return;
        }

        LRP_4B5B_addEncodedByteToBufferOfEncodedBits(lineCode4B5B, data);

        if (LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(lineCode4B5B)) {
            const unsigned char const decodedData = LRP_4B5B_readADecodedByteFromBufferOfEncodedBits(lineCode4B5B);
            LRP_ReceiveLinkLayer_handler(sessionProvider, &decodedData);
        }
    }
}