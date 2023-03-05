#include <gtest/gtest.h>
#include "../../../src/transmitter/layers/validator.layer.h"
#include "../../../src/transmitter/transmitter.module.h"

class TransmitterValidatorLayerTest : public ::testing::Test {
protected:

    LRPTransmitterSessionProvider transmitterSessionProvider{};
    unsigned char sourceDeviceId = 0b10100u;
    const unsigned char targetId = 0b11001u;
    const unsigned char command = LRP_FRAME_NO_COMMAND;
    const unsigned char length = 0b011u;
    LRPFrame frameBuffer[3]{};
    unsigned char data0 = 'L';
    unsigned char data1 = 'R';
    unsigned char data2 = 'P';


    void SetUp() override {
        LRP_Transmitter_init(&transmitterSessionProvider, &sourceDeviceId, frameBuffer, 3u);

        transmitterSessionProvider.validatorCurrentFrame->length = 3u;

        transmitterSessionProvider.validatorCurrentFrame->data[0] = &data0;
        transmitterSessionProvider.validatorCurrentFrame->data[1] = &data1;
        transmitterSessionProvider.validatorCurrentFrame->data[2] = &data2;

        transmitterSessionProvider.validatorCurrentFrame->command = command;
        transmitterSessionProvider.validatorCurrentFrame->sourceDeviceId = sourceDeviceId;
        transmitterSessionProvider.validatorCurrentFrame->targetId = targetId;
    }

    void TearDown() override {
        ASSERT_EQ(transmitterSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*transmitterSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
        ASSERT_EQ(transmitterSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);

        ASSERT_EQ(transmitterSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(TransmitterValidatorLayerTest, when_the_status_is_not_ready_to_check_should_be_handled_right) {
    LRP_TransmitterValidatorLayer_handler(&transmitterSessionProvider);

    // Header 1
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->buffer[0], 0u);
    // Header 2
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->buffer[1], 0u);
    // Data 1
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->buffer[2], 0u);
    // Data 2
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->buffer[3], 0u);
    // Data 3
    ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame->buffer[4], 0u);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(transmitterSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
}

TEST_F(TransmitterValidatorLayerTest, when_the_status_is_ready_to_check_should_be_handled_right) {
    transmitterSessionProvider.validatorCurrentFrame->status = LRP_TRANSMITTER_FRAME_READY_TO_CHECK;

    LRP_TransmitterValidatorLayer_handler(&transmitterSessionProvider);

    // Header 1
    ASSERT_EQ(frameBuffer[0].buffer[0], 0b11001000u);
    // Header 2
    ASSERT_EQ(frameBuffer[0].buffer[1], 0b10100011u);
    // Data 1
    ASSERT_EQ(frameBuffer[0].buffer[2], data0);
    // Data 2
    ASSERT_EQ(frameBuffer[0].buffer[3], data1);
    // Data 3
    ASSERT_EQ(frameBuffer[0].buffer[4], data2);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(transmitterSessionProvider.validatorCurrentFrame, &frameBuffer[1]);
}
