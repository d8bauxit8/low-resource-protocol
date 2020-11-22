#include <gtest/gtest.h>
#include "../../src/transmit/transmit_application_layer.h"

class TransmitApplicationLayerTest : public ::testing::Test {
protected:

    LRPTransmitSessionProvider transmitSessionProvider{};
    const unsigned char deviceId = 20;
    LRPFrame frameBuffer[3]{};

    void SetUp() override {
        LRP_SessionProvider_init((LRPSessionProvider *) &transmitSessionProvider, &deviceId, frameBuffer, 3);
    }

    void TearDown() override {
        ASSERT_EQ(transmitSessionProvider.deviceId, &deviceId);
        ASSERT_EQ(*transmitSessionProvider.deviceId, deviceId);

        ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
        ASSERT_EQ(transmitSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);

        ASSERT_EQ(transmitSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(transmitSessionProvider.linkCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(transmitSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(TransmitApplicationLayerTest, Should_Be_Set_Data_Into_Reserved_Frame) {
    unsigned char data[] = "LRP";
    unsigned char lengthOfData = 3;

    LRP_TransmitApplicationLayer_setDataIntoReservedFrame(&transmitSessionProvider, data,
                                                          lengthOfData);

    ASSERT_EQ(*transmitSessionProvider.applicationCurrentFrame->data[0], 'L');
    ASSERT_EQ(*transmitSessionProvider.applicationCurrentFrame->data[1], 'R');
    ASSERT_EQ(*transmitSessionProvider.applicationCurrentFrame->data[2], 'P');
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->data[3], nullptr);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->length, 3);

    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->command, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->targetDeviceId, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->status, FRAME_READY_TO_REDEFINE);
}

TEST_F(TransmitApplicationLayerTest, Reserved_Frame_Should_Be_Transmited) {
    const unsigned char targetDeviceId = 25;
    const unsigned char command = 0b101;

    LRP_TransmitApplicationLayer_transmitReservedFrame(&transmitSessionProvider,
                                                       targetDeviceId, command);

    ASSERT_EQ(frameBuffer[0].targetDeviceId, targetDeviceId);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, deviceId);
    ASSERT_EQ(frameBuffer[0].command, command);

    ASSERT_EQ(frameBuffer[0].status, TRANSMIT_FRAME_READY_TO_CHECK);
    ASSERT_EQ(frameBuffer[0].length, 0);
    ASSERT_EQ(frameBuffer[0].data[0], nullptr);

    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->command, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->targetDeviceId, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->status, FRAME_READY_TO_REDEFINE);
}

TEST_F(TransmitApplicationLayerTest, Should_Be_Set_Ready_To_Redefine_Frame_To_Reserved) {
    unsigned char status = LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(&transmitSessionProvider);

    ASSERT_TRUE(status);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->status, TRANSMIT_FRAME_RESERVED);

    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->command, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->targetDeviceId, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->data[0], nullptr);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->length, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame, &frameBuffer[0]);

    status = LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(&transmitSessionProvider);
    ASSERT_FALSE(status);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->status, TRANSMIT_FRAME_RESERVED);

    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->command, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->targetDeviceId, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->data[0], nullptr);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame->length, 0);
    ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}