#include "testing.h"

#define BUFFER_TEST_DATA_LENGTH 10
unsigned char bufferTest[] = {START_DELIMITER_BYTE_4B5B, 0b01110111, 0b00101010, 0b10100111, 0b11010110, 0b01010010,
                              0b01110101, 0b10101001, 0b00000101,
                              END_DELIMITER_BYTE_4B5B};

#define BUFFER_NOPE_DATA_LENGTH 10
unsigned char bufferNope[] = {START_DELIMITER_BYTE_4B5B, 0b01110111, 0b00101010, 0b11000111, 0b01010101, 0b01110111,
                              0b11111110, 0b00101101, 0b00000111,
                              END_DELIMITER_BYTE_4B5B};

const unsigned char const deviceId = 0b00010011;
#define FRAME_BUFFER_LENGTH 3

void receive_TEST_1(void) {
    _LRPReceiveSessionProvider receive;

    _LRPFrame receiveFrameBuffer[FRAME_BUFFER_LENGTH];

    LRP_SessionProvider_init(&receive, &deviceId, receiveFrameBuffer, FRAME_BUFFER_LENGTH);

    test_readData(&receive, bufferTest, BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_printReceiveLayer(&receive, FRAME_BUFFER_LENGTH);

    _LRPReceiveFrameController controllers[] = {test_receiveFrameController};

    LRP_ReceiveApplicationLayer_controller(&receive, controllers, 1);
}

void receive_TEST_2(void) {
    _LRPReceiveSessionProvider receive;

    _LRPFrame receiveFrameBuffer[FRAME_BUFFER_LENGTH];

    LRP_SessionProvider_init(&receive, &deviceId, receiveFrameBuffer, FRAME_BUFFER_LENGTH);

    test_readData(&receive, bufferTest, BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_readData(&receive, bufferTest, BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_readData(&receive, bufferTest, BUFFER_TEST_DATA_LENGTH);

    test_readData(&receive, bufferNope, BUFFER_NOPE_DATA_LENGTH);
    test_readData(&receive, bufferNope, BUFFER_NOPE_DATA_LENGTH);

    test_printReceiveLayer(&receive, FRAME_BUFFER_LENGTH);
}

void receive_TEST_3(void) {
    _LRPReceiveSessionProvider receive;

    _LRPFrame receiveFrameBuffer[FRAME_BUFFER_LENGTH];

    LRP_SessionProvider_init(&receive, &deviceId, receiveFrameBuffer, FRAME_BUFFER_LENGTH);

    test_readData(&receive, bufferTest, BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_readData(&receive, bufferTest, BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_readData(&receive, bufferTest, BUFFER_TEST_DATA_LENGTH);

    test_readData(&receive, bufferNope, BUFFER_NOPE_DATA_LENGTH);
    test_readData(&receive, bufferNope, BUFFER_NOPE_DATA_LENGTH);

    _LRPReceiveFrameController controllers[] = {test_receiveFrameController};
    LRP_ReceiveApplicationLayer_controller(&receive, controllers, 1);

    test_printReceiveLayer(&receive, FRAME_BUFFER_LENGTH);

}

int main() {
    describe("TEST 1 - Receiving flow testing", receive_TEST_1);
    describe("TEST 2 - Overflow buffer testing", receive_TEST_2);
    describe("TEST 2 - Overflow buffer testing", receive_TEST_3);

    return 0;
}
