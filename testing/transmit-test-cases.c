#include "transmit-test-cases.h"

#define T_BUFFER_TEST_DATA_LENGTH 4
unsigned char t_bufferTest[] = "Test";

#define T_BUFFER_NOPE_DATA_LENGTH 4
unsigned char t_bufferNope[] = "Nope";

const unsigned char const t_sourceDeviceId = 0b00001100;
const unsigned char const t_targetDeviceId = 0b00010011;
#define T_FRAME_BUFFER_LENGTH 3

void t_case_1(void) {
    // Initialization needed variables
    _LRPTransmitSessionProvider transmit;
    _LRPFrame transmitFrameBuffer[T_FRAME_BUFFER_LENGTH];

    // Initialization session provider
    LRP_SessionProvider_init(&transmit, &t_sourceDeviceId, transmitFrameBuffer, T_FRAME_BUFFER_LENGTH);

    // Add data into buffer
    LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(&transmit);
    LRP_TransmitApplicationLayer_setDataIntoReservedFrame(&transmit, t_bufferTest, T_BUFFER_TEST_DATA_LENGTH);
    LRP_TransmitApplicationLayer_transmitReservedFrame(&transmit, t_targetDeviceId, 0);
    test_printTransmitLayer(&transmit, T_FRAME_BUFFER_LENGTH);
}

void t_case_2(void) {
    // Initialization needed variables
    _LRPTransmitSessionProvider transmit;
    _LRPFrame transmitFrameBuffer[T_FRAME_BUFFER_LENGTH];

    // Initialization session provider
    LRP_SessionProvider_init(&transmit, &t_sourceDeviceId, transmitFrameBuffer, T_FRAME_BUFFER_LENGTH);

    // Add data into buffer
    LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(&transmit);
    LRP_TransmitApplicationLayer_setDataIntoReservedFrame(&transmit, t_bufferTest, T_BUFFER_TEST_DATA_LENGTH);
    LRP_TransmitApplicationLayer_transmitReservedFrame(&transmit, t_targetDeviceId, 0);

    LRP_TransmitValidatorLayer_handler(&transmit);
    test_printTransmitLayer(&transmit, T_FRAME_BUFFER_LENGTH);
}

void test_transmit(void) {
    test("TEST 1 - Transmit application layer testing", t_case_1);
    test("TEST 2 - Transmit application layer and validation", t_case_2);
}