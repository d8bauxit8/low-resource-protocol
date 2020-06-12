#include "receive-test-cases.h"

void r_case_1(void) {
    // Initialization needed variables
    _LRPReceiveSessionProvider sessionProvider;
    _LRPFrame receiveFrameBuffer[FRAME_BUFFER_LENGTH];

    // Initialization session provider
    LRP_SessionProvider_init(&sessionProvider, &targetDeviceId, receiveFrameBuffer, FRAME_BUFFER_LENGTH);

    // Read a data flow then validate it
    test_receiveADataFlow(&sessionProvider, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&sessionProvider);

    // Print the receive layer
    test_printReceiveLayer(&sessionProvider, FRAME_BUFFER_LENGTH);

    // Create a frame controller and call the application layer to run the frame controller
    _LRPReceiveFrameController controllers[] = {test_receiveFrameController};
    LRP_ReceiveApplicationLayer_controller(&sessionProvider, controllers, 1);
}

void r_case_2(void) {
    // Initialization needed variables
    _LRPReceiveSessionProvider sessionProvider;
    _LRPFrame receiveFrameBuffer[FRAME_BUFFER_LENGTH];

    // Initialization session provider
    LRP_SessionProvider_init(&sessionProvider, &targetDeviceId, receiveFrameBuffer, FRAME_BUFFER_LENGTH);

    // Read a data flow then validate it
    test_receiveADataFlow(&sessionProvider, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&sessionProvider);

    // Read one more data flow then validate it
    test_receiveADataFlow(&sessionProvider, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&sessionProvider);

    // Read a data flow
    test_receiveADataFlow(&sessionProvider, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);

    // Try to read the following two data flow
    test_receiveADataFlow(&sessionProvider, r_bufferNope, R_BUFFER_NOPE_DATA_LENGTH);
    test_receiveADataFlow(&sessionProvider, r_bufferNope, R_BUFFER_NOPE_DATA_LENGTH);

    // Print the receive layer
    test_printReceiveLayer(&sessionProvider, FRAME_BUFFER_LENGTH);
}

void r_case_3(void) {
    // Initialization needed variables
    _LRPReceiveSessionProvider sessionProvider;
    _LRPFrame receiveFrameBuffer[FRAME_BUFFER_LENGTH];

    // Initialization session provider
    LRP_SessionProvider_init(&sessionProvider, &targetDeviceId, receiveFrameBuffer, FRAME_BUFFER_LENGTH);

    // Read a data flow then validate it
    test_receiveADataFlow(&sessionProvider, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&sessionProvider);

    // Read one more data flow then validate it
    test_receiveADataFlow(&sessionProvider, r_bufferTest, R_BUFFER_TEST_DATA_LENGTH);
    LRP_ReceiveValidatorLayer_handler(&sessionProvider);

    // Create a frame controller and call the application layer to run the frame controller
    _LRPReceiveFrameController controllers[] = {test_receiveFrameController};
    LRP_ReceiveApplicationLayer_controller(&sessionProvider, controllers, 1);

    // Print the receive layer
    test_printReceiveLayer(&sessionProvider, FRAME_BUFFER_LENGTH);
}

void test_receive(void) {
    test("TEST 1 - Receiving flow testing", r_case_1);
    test("TEST 2 - Overflow buffer testing", r_case_2);
    test("TEST 3 - Receiving and redefine the first buffer", r_case_3);
}