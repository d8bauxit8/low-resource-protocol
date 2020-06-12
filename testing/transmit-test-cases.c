#include "transmit-test-cases.h"
#include "stdio.h"

void t_case_1(void) {
    // Initialization needed variables
    _LRPTransmitSessionProvider sessionProvider;
    _LRPFrame transmitFrameBuffer[FRAME_BUFFER_LENGTH];

    // Initialization session provider
    LRP_SessionProvider_init(&sessionProvider, &sourceDeviceId, transmitFrameBuffer, FRAME_BUFFER_LENGTH);

    // Add data into buffer
    LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(&sessionProvider);
    LRP_TransmitApplicationLayer_setDataIntoReservedFrame(&sessionProvider, t_bufferTest, T_BUFFER_TEST_DATA_LENGTH);
    LRP_TransmitApplicationLayer_transmitReservedFrame(&sessionProvider, targetDeviceId, command);
    test_printTransmitLayer(&sessionProvider, FRAME_BUFFER_LENGTH);
}

void t_case_2(void) {
    // Initialization needed variables
    _LRPTransmitSessionProvider sessionProvider;
    _LRPFrame transmitFrameBuffer[FRAME_BUFFER_LENGTH];

    // Initialization session provider
    LRP_SessionProvider_init(&sessionProvider, &sourceDeviceId, transmitFrameBuffer, FRAME_BUFFER_LENGTH);

    // Add data into buffer
    LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(&sessionProvider);
    LRP_TransmitApplicationLayer_setDataIntoReservedFrame(&sessionProvider, t_bufferTest, T_BUFFER_TEST_DATA_LENGTH);
    LRP_TransmitApplicationLayer_transmitReservedFrame(&sessionProvider, targetDeviceId, command);

    LRP_TransmitValidatorLayer_handler(&sessionProvider);
    test_printTransmitLayer(&sessionProvider, FRAME_BUFFER_LENGTH);
}

void t_case_3(void) {
    // Initialization needed variables
    _LRPTransmitSessionProvider sessionProvider;
    _LRPFrame transmitFrameBuffer[FRAME_BUFFER_LENGTH];

    // Initialization session provider
    LRP_SessionProvider_init(&sessionProvider, &sourceDeviceId, transmitFrameBuffer, FRAME_BUFFER_LENGTH);

    // Add data into buffer
    LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(&sessionProvider);
    LRP_TransmitApplicationLayer_setDataIntoReservedFrame(&sessionProvider, t_bufferTest, T_BUFFER_TEST_DATA_LENGTH);
    LRP_TransmitApplicationLayer_transmitReservedFrame(&sessionProvider, targetDeviceId, command);

    LRP_TransmitValidatorLayer_handler(&sessionProvider);

    _LRPLineCode4B5B lineCode4B5B;
    unsigned char buffer[2] = {0, 0};
    lineCode4B5B.buffer[0] = &buffer[0];
    lineCode4B5B.buffer[1] = &buffer[1];
    unsigned char data[R_BUFFER_TEST_DATA_LENGTH];
    unsigned char i = 0;
    LRP_TransmitLineCodeLayer_ifThereIsNoTransmittingSendTheStartingDelimiterByte4B5B(&sessionProvider, &lineCode4B5B,
                                                                                      &data[i++]);
    for (; i < R_BUFFER_TEST_DATA_LENGTH; i++) {
        LRP_TransmitLineCodeLayer_handler(&sessionProvider, &lineCode4B5B, &data[i]);
    }

    test_printCheckTransmitDataValidity(data);

    test_printTransmitLayer(&sessionProvider, FRAME_BUFFER_LENGTH);
}

void test_transmit(void) {
    test("TEST 1 - Transmit application layer testing", t_case_1);
    test("TEST 2 - Transmit application layer and validation", t_case_2);
    test("TEST 3 - Transmit application layer and validation", t_case_3);
}