#include "transmit-test-cases.h"

#define T_BUFFER_TEST_DATA_LENGTH 4
unsigned char t_bufferTest[] = "Test";

#define T_BUFFER_NOPE_DATA_LENGTH 4
unsigned char t_bufferNope[] = "Nope";

const unsigned char const t_deviceId = 0b00010011;
#define T_FRAME_BUFFER_LENGTH 3

void t_case_1(void) {
    _LRPTransmitSessionProvider transmit;

    _LRPFrame transmitFrameBuffer[T_FRAME_BUFFER_LENGTH];

    LRP_SessionProvider_init(&transmit, &t_deviceId, transmitFrameBuffer, T_FRAME_BUFFER_LENGTH);

    test_writeData(&transmit, t_bufferTest, T_BUFFER_TEST_DATA_LENGTH, t_deviceId, 0x00);

    test_printTransmitLayer(&transmit, T_FRAME_BUFFER_LENGTH);
}

void test_transmit(void) {
    test("TEST 1 - Transmit flow testing", t_case_1);
}