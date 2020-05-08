#include "receive_line_code_layer.h"

void LRP_receiveLineCodeLayerHandler(_LRPReceiveLayer *const receiveLayer, _LRPLineCode4B5B *const lineCode4B5B,
                                     const unsigned char *const data) {
    if (*data == START_DELIMITER_BYTE_4B5B) {
        LRP_receiveLinkLayerStartReceiving(receiveLayer);
        LRP_4B5BReset(lineCode4B5B);
        return;
    }

    if (LRP_isReceiveLinkLayerStatusOK(receiveLayer)) {
        if (*data == END_DELIMITER_BYTE_4B5B) {
            LRP_receiveLinkLayerEndReceiving(receiveLayer);
            return;
        }

        LRP_4B5BAddEncodedByteToBufferOfEncodedBits(lineCode4B5B, data);

        if (LRP_4B5BIsBufferOfEncodedBitsReadyToReadADecodedByte(lineCode4B5B)) {
            const unsigned char const decodedData = LRP_4B5BReadADecodedByteFromBufferOfEncodedBits(lineCode4B5B);
            LRP_receiveLinkLayerHandler(receiveLayer, &decodedData);
        }
    }
}