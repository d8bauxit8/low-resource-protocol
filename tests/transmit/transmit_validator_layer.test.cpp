#include <gtest/gtest.h>
#include "../../src/transmit/transmit_validator_layer.h"

class TransmitValidatorLayerTest : public ::testing::Test {
protected:

    LRPTransmitSessionProvider transmitSessionProvider{};
    const unsigned char deviceId = 0b10100;
    LRPFrame frameBuffer[3]{};
    unsigned char data0 = 'L';
    unsigned char data1 = 'R';
    unsigned char data2 = 'P';


    void SetUp() override {
        LRP_SessionProvider_init((LRPSessionProvider *) &transmitSessionProvider, &deviceId, frameBuffer, 3);

        transmitSessionProvider.validatorCurrentFrame->length = 3;

        transmitSessionProvider.validatorCurrentFrame->data[0] = &data0;
        transmitSessionProvider.validatorCurrentFrame->data[1] = &data1;
        transmitSessionProvider.validatorCurrentFrame->data[2] = &data2;

        transmitSessionProvider.validatorCurrentFrame->command = 0b101;
        transmitSessionProvider.validatorCurrentFrame->sourceDeviceId = deviceId;
        transmitSessionProvider.validatorCurrentFrame->targetDeviceId = 0b11001;
    }

    void TearDown() override {
        ASSERT_EQ(transmitSessionProvider.deviceId, &deviceId);
        ASSERT_EQ(*transmitSessionProvider.deviceId, deviceId);

        ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
        ASSERT_EQ(transmitSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);

        ASSERT_EQ(transmitSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(transmitSessionProvider.linkCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(TransmitValidatorLayerTest, Should_Be_Handled_When_The_Status_Is_Not_Ready_To_Check) {
    LRP_TransmitValidatorLayer_handler(&transmitSessionProvider);

    // Header 1
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->buffer[0], 0);
    // Header 2
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->buffer[1], 0);
    // Data 1
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->buffer[2], 0);
    // Data 2
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->buffer[3], 0);
    // Data 3
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->buffer[4], 0);

    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(transmitSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
}

TEST_F(TransmitValidatorLayerTest, Should_Be_Handled_When_The_Status_Is_Ready_To_Check) {
    transmitSessionProvider.validatorCurrentFrame->status = TRANSMIT_FRAME_READY_TO_CHECK;

    LRP_TransmitValidatorLayer_handler(&transmitSessionProvider);

    // Header 1
    ASSERT_EQ(frameBuffer[0].buffer[0], 0b11001101);
    // Header 2
    ASSERT_EQ(frameBuffer[0].buffer[1], 0b10100011);
    // Data 1
    ASSERT_EQ(frameBuffer[0].buffer[2], data0);
    // Data 2
    ASSERT_EQ(frameBuffer[0].buffer[3], data1);
    // Data 3
    ASSERT_EQ(frameBuffer[0].buffer[4], data2);

    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(transmitSessionProvider.validatorCurrentFrame, &frameBuffer[1]);
}
