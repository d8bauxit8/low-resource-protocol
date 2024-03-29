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

        ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
        ASSERT_EQ(transmitterSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);

        ASSERT_EQ(transmitterSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(transmitterSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(TransmitterApplicationLayerTest, data_should_be_set_into_reserved_frame_right) {
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

TEST_F(TransmitterApplicationLayerTest, reserved_frame_should_be_transmited) {
    const unsigned char targetId = 0b11001u;
    const unsigned char command = LRP_FRAME_NO_COMMAND;

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

TEST_F(TransmitterApplicationLayerTest, ready_to_redefine_frame_should_be_reserved) {
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