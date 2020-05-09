#include "testing.h"

void receiveLayer_TEST_1(void) {
    _LRPSessionProvider receive;

    unsigned char deviceId = 0b00010011;
    unsigned char frameBufferLength = 3;

    unsigned char bufferTest[] = {START_DELIMITER_BYTE_4B5B, 0b01110111, 0b00101010, 0b10100111, 0b11010110, 0b01010010,
                                  0b01110101, 0b10101001, 0b00000101,
                                  END_DELIMITER_BYTE_4B5B};

    _LRPFrame receiveFrameBuffer[3];

    LRP_initSessionProvider(&receive, &deviceId, receiveFrameBuffer, frameBufferLength);

    test_sendData(&receive, bufferTest, 10);
    LRP_receiveValidatorLinkLayerHandler(&receive);

    test_printReceiveLayer(&receive, frameBufferLength);

    _LRPReceiveFrameController controllers[1] = {test_receiveFrameController};

    LRP_receiveApplicationLayerController(&receive, controllers, 1);
}

void receiveLayer_TEST_2(void) {
    _LRPSessionProvider receive;

    unsigned char deviceId = 0b00010011;
    unsigned char frameBufferLength = 3;

    unsigned char bufferTest[] = {START_DELIMITER_BYTE_4B5B, 0b01110111, 0b00101010, 0b10100111, 0b11010110, 0b01010010,
                                  0b01110101, 0b10101001, 0b00000101,
                                  END_DELIMITER_BYTE_4B5B};

    unsigned char bufferNope[] = {START_DELIMITER_BYTE_4B5B, 0b01110111, 0b00101010, 0b11000111, 0b01010101, 0b01010111,
                                  0b01111110, 0b00101101, 0b00000101,
                                  END_DELIMITER_BYTE_4B5B};

    _LRPFrame receiveFrameBuffer[3];

    LRP_initSessionProvider(&receive, &deviceId, receiveFrameBuffer, frameBufferLength);

    test_sendData(&receive, bufferTest, 10);
    LRP_receiveValidatorLinkLayerHandler(&receive);

    test_sendData(&receive, bufferTest, 10);
    LRP_receiveValidatorLinkLayerHandler(&receive);

    test_sendData(&receive, bufferTest, 10);
    test_sendData(&receive, bufferNope, 10);
    test_sendData(&receive, bufferNope, 10);

    test_printReceiveLayer(&receive, frameBufferLength);
}

int main() {
    describe("TEST 1 - Receiving flow testing", receiveLayer_TEST_1);
    describe("TEST 2 - Overflow buffer testing", receiveLayer_TEST_2);
    return 0;
}
