#include "testing.h"
#include "src/receive_controller.h"

void receiveLayer_TEST_1(void) {
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
    LRP_receiveLayerHandler(&receive, target, &parityBitByte, &framingErrorByte, &overrunErrorByte);

    test_printReceiveLayer(&receive, frameBufferLength);
}

void receiveLayer_TEST_2(void) {
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

    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Tst1");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Tst2");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Tst3");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Nope");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Nope");

    test_printReceiveLayer(&receive, frameBufferLength);
}

void receiveLayer_TEST_3(void) {
    _LRPReceiveLayer receive;

    unsigned char deviceId = 0b00000011;
    unsigned char target = 0b00111000;
    unsigned char broadcast = 0b11111000;
    unsigned char source = 0b00001000;
    unsigned char framingErrorByte = 0b00000000, overrunErrorByte = 0b00000000;

    unsigned char frameBufferLength = 5;
    _LRPFrame receiveFrameBuffer[5];

    LRP_initReceiveLayer(&receive, &deviceId, receiveFrameBuffer, frameBufferLength);

    LRP_setFramingErrorHandler(&receive, test_framingError);
    LRP_setOverrunErrorHandler(&receive, test_overrunError);
    LRP_setParityBitErrorHandler(&receive, test_parityBitError);

    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Nope");
    test_sendData(&receive, broadcast, source, framingErrorByte, overrunErrorByte, "Brc1");
    test_sendData(&receive, broadcast, source, framingErrorByte, overrunErrorByte, "Brc2");
    test_sendData(&receive, target, source, framingErrorByte, overrunErrorByte, "Nope");
    test_sendData(&receive, broadcast, source, framingErrorByte, overrunErrorByte, "Brc3");

    test_printReceiveLayer(&receive, frameBufferLength);
}

int main() {
    describe("TEST 1 - Buffer testing", receiveLayer_TEST_1);
    describe("TEST 2 - Overflow buffer testing", receiveLayer_TEST_2);
    describe("TEST 3 - Broadcast testing", receiveLayer_TEST_3);
    return 0;
}
