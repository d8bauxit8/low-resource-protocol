#include "receive-test-cases.h"

#define R_BUFFER_TEST_DATA_LENGTH 10
unsigned char r_bufferTest[] = {START_DELIMITER_BYTE_4B5B, 0b01110111, 0b00101010, 0b10100111, 0b11010110, 0b01010010,
                                0b01110101, 0b10101001, 0b00000101,
                                END_DELIMITER_BYTE_4B5B};

#define R_BUFFER_NOPE_DATA_LENGTH 10
unsigned char r_bufferNope[] = {START_DELIMITER_BYTE_4B5B, 0b01110111, 0b00101010, 0b11000111, 0b01010101, 0b01110111,
                                0b11111110, 0b00101101, 0b00000111,
                                END_DELIMITER_BYTE_4B5B};

const unsigned char const r_deviceId = 0b00010011;
#define R_FRAME_BUFFER_LENGTH 3

void r_case_1(void) {
    _LRPReceiveSessionProvider receive;

    _LRPFrame receiveFrameBuffer[R_FRAME_BUFFER_LENGTH];

    LRP_SessionProvider_init(&receive, &r_deviceId, receiveFrameBuffer, R_FRAME_BUFFER_LENGTH);

    test_readData(&receive, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_printReceiveLayer(&receive, R_FRAME_BUFFER_LENGTH);

    _LRPReceiveFrameController controllers[] = {test_receiveFrameController};

    LRP_ReceiveApplicationLayer_controller(&receive, controllers, 1);
}

void r_case_2(void) {
    _LRPReceiveSessionProvider receive;

    _LRPFrame receiveFrameBuffer[R_FRAME_BUFFER_LENGTH];

    LRP_SessionProvider_init(&receive, &r_deviceId, receiveFrameBuffer, R_FRAME_BUFFER_LENGTH);

    test_readData(&receive, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_readData(&receive, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_readData(&receive, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);

    test_readData(&receive, r_bufferNope, R_BUFFER_NOPE_DATA_LENGTH);
    test_readData(&receive, r_bufferNope, R_BUFFER_NOPE_DATA_LENGTH);

    test_printReceiveLayer(&receive, R_FRAME_BUFFER_LENGTH);
}

void r_case_3(void) {
    _LRPReceiveSessionProvider receive;

    _LRPFrame receiveFrameBuffer[R_FRAME_BUFFER_LENGTH];

    LRP_SessionProvider_init(&receive, &r_deviceId, receiveFrameBuffer, R_FRAME_BUFFER_LENGTH);

    test_readData(&receive, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_readData(&receive, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&receive);

    test_readData(&receive, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);

    test_readData(&receive, r_bufferNope, R_BUFFER_NOPE_DATA_LENGTH);
    test_readData(&receive, r_bufferNope, R_BUFFER_NOPE_DATA_LENGTH);

    _LRPReceiveFrameController controllers[] = {test_receiveFrameController};
    LRP_ReceiveApplicationLayer_controller(&receive, controllers, 1);

    test_printReceiveLayer(&receive, R_FRAME_BUFFER_LENGTH);
}

void test_receive(void) {
    test("TEST 1 - Receiving flow testing", r_case_1);
    test("TEST 2 - Overflow buffer testing", r_case_2);
    test("TEST 3 - Receiving and redefine the first buffer", r_case_3);
}