#include "receive_line_code_layer.h"

/**
 * Public method declarations
 */
void LRP_receiveLineCodeLayerHandler(_LRPSessionProvider *const sessionProvider, _LRPLineCode4B5B *const lineCode4B5B,
                                     const unsigned char *const data) {
    if (*data == START_DELIMITER_BYTE_4B5B) {
        LRP_receiveLinkLayerStartReceiving(sessionProvider);
        LRP_4B5BReset(lineCode4B5B);
        return;
    }

    if (LRP_isReceiveLinkLayerStatusOK(sessionProvider)) {
        if (*data == END_DELIMITER_BYTE_4B5B) {
            LRP_receiveLinkLayerEndReceiving(sessionProvider);
            return;
        }

        LRP_4B5BAddEncodedByteToBufferOfEncodedBits(lineCode4B5B, data);

        if (LRP_4B5BIsBufferOfEncodedBitsReadyToReadADecodedByte(lineCode4B5B)) {
            const unsigned char const decodedData = LRP_4B5BReadADecodedByteFromBufferOfEncodedBits(lineCode4B5B);
            LRP_receiveLinkLayerHandler(sessionProvider, &decodedData);
        }
    }
}