#include "testing.h"
#include "src/receive_controller.h"

void receiveLayer_TEST_1(void) {
    _LRPReceiveLayer receive;

    unsigned char deviceId = 0b00000011;
    unsigned char target = 0b00011000;
    unsigned char source = 0b00001000;
    unsigned char framingErrorByte = 0b00000000, overrunErrorByte = 0b00000000;

    unsigned char frameBufferLength = 3;
    _LRPFrame receiveFrameBuffer[3];

    LRP_initReceiveLayer(&receive, &deviceId, receiveFrameBuffer, frameBufferLength);

    LRP_setFramingErrorHandler(&receive, test_framingError);
    LRP_setOverrunErrorHandler(&receive, test_overrunError);
    LRP_setParityBitErrorHandler(&receive, test_parityBitError);

    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Hell");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "o wo");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "rld!");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Mark");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Test");

    test_printReceiveLayer(&receive);
}

void receiveLayer_TEST_2(void) {
    _LRPReceiveLayer receive;

    unsigned char deviceId = 0b00000011;
    unsigned char target = 0b00011000;
    unsigned char source = 0b00001000;
    unsigned char parityBitByte;
    unsigned char framingErrorByte = 0b00000000, overrunErrorByte = 0b00000000;

    unsigned char frameBufferLength = 3;
    _LRPFrame receiveFrameBuffer[3];

    LRP_initReceiveLayer(&receive, &deviceId, receiveFrameBuffer, frameBufferLength);

    LRP_setFramingErrorHandler(&receive, test_framingError);
    LRP_setOverrunErrorHandler(&receive, test_overrunError);
    LRP_setParityBitErrorHandler(&receive, test_parityBitError);

    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Test");

    parityBitByte = test_generateParity(target);
    LRP_receiveLayerController(&receive, target, &parityBitByte, &framingErrorByte, &overrunErrorByte);

    test_printReceiveLayer(&receive);

}

int main() {
    describe("TEST 1", receiveLayer_TEST_1);
    describe("TEST 2", receiveLayer_TEST_2);
    return 0;
}
