#include <gtest/gtest.h>
#include "../../../src/transmitter/layers/application.layer.h"
#include "../../../src/transmitter/transmitter.module.h"

class TransmitterApplicationLayerTest : public ::testing::Test {
protected:

    LRPTransmitterSessionProvider transmitterSessionProvider{};
    unsigned char sourceDeviceId = 0b10100u;
    LRPFrame frameBuffer[3]{};

    void SetUp() override {
        LRP_Transmitter_init(&transmitterSessionProvider, &sourceDeviceId, frameBuffer, 3u);
    }

    void TearDown() override {
        ASSERT_EQ(transmitterSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*transmitterSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRPLinkLayerStatus_Skip);
        ASSERT_EQ(transmitterSessionProvider.linkLayerErrorCode, LRPLinkLayerErrorCode_NoError);

        ASSERT_EQ(transmitterSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(transmitterSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(TransmitterApplicationLayerTest, Should_Be_Set_Data_Into_Reserved_Frame) {
    unsigned char data[] = "LRP";
    unsigned char lengthOfData = 3u;

    LRP_TransmitterApplicationLayer_setDataIntoReservedFrame(&transmitterSessionProvider, data,
                                                             lengthOfData);

    ASSERT_EQ(*transmitterSessionProvider.applicationCurrentFrame->data[0], 'L');
    ASSERT_EQ(*transmitterSessionProvider.applicationCurrentFrame->data[1], 'R');
    ASSERT_EQ(*transmitterSessionProvider.applicationCurrentFrame->data[2], 'P');
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->data[3], nullptr);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->length, 3);

    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->command, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->targetId, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->status, LRP_FRAME_READY_TO_REDEFINE);
}

TEST_F(TransmitterApplicationLayerTest, Reserved_Frame_Should_Be_Transmited) {
    const unsigned char targetId = 0b11001u;
    const LRPFrameCommand command = LRPFrameCommand_NoCommand;

    LRP_TransmitterApplicationLayer_transmitReservedFrame(&transmitterSessionProvider,
                                                          targetId, command);

    ASSERT_EQ(frameBuffer[0].targetId, targetId);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, sourceDeviceId);
    ASSERT_EQ(frameBuffer[0].command, command);

    ASSERT_EQ(frameBuffer[0].status, LRP_TRANSMITTER_FRAME_READY_TO_CHECK);
    ASSERT_EQ(frameBuffer[0].length, 0);
    ASSERT_EQ(frameBuffer[0].data[0], nullptr);

    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->command, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->targetId, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->status, LRP_FRAME_READY_TO_REDEFINE);
}

TEST_F(TransmitterApplicationLayerTest, Should_Be_Set_Ready_To_Redefine_Frame_To_Reserved) {
    unsigned char status = LRP_TransmitterApplicationLayer_setReadyToRedefineFrameToReserved(
            &transmitterSessionProvider);

    ASSERT_TRUE(status);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->status, LRP_TRANSMITTER_FRAME_RESERVED);

    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->command, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->targetId, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->data[0], nullptr);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->length, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame, &frameBuffer[0]);

    status = LRP_TransmitterApplicationLayer_setReadyToRedefineFrameToReserved(&transmitterSessionProvider);
    ASSERT_FALSE(status);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->status, LRP_TRANSMITTER_FRAME_RESERVED);

    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->command, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->targetId, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->data[0], nullptr);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->length, 0);
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}