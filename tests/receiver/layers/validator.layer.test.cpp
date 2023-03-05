#include <gtest/gtest.h>
#include "../../../src/receiver/layers/validator.layer.h"
#include "../../../src/receiver/receiver.module.h"

class ReceiverValidatorLayerTest : public ::testing::Test {
protected:

    LRPReceiverSessionProvider receiverSessionProvider{};
    unsigned char sourceDeviceId = 0b10100u;
    unsigned char groupId = 0b11100u;
    const unsigned char targetId = 0b11001u;
    const unsigned char command = 0b101u;
    const unsigned char length = 0b011u;
    LRPFrame frameBuffer[3]{};
    unsigned char data0 = 'L';
    unsigned char data1 = 'R';
    unsigned char data2 = 'P';

    void SetUp() override {
        frameBuffer[0].buffer[0] = (unsigned char) (targetId << 3u) | command;
        frameBuffer[0].buffer[1] = (unsigned char) (sourceDeviceId << 3u) | length;
        frameBuffer[0].buffer[2] = data0;
        frameBuffer[0].buffer[3] = data1;
        frameBuffer[0].buffer[4] = data2;

        LRP_Receiver_init(&receiverSessionProvider, &sourceDeviceId, frameBuffer, 3, &groupId);
    }

    void TearDown() override {
        ASSERT_EQ(receiverSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*receiverSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
        ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);

        ASSERT_EQ(receiverSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(receiverSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(ReceiverValidatorLayerTest,
       when_the_status_is_ready_to_check_should_be_handled_right) {
    receiverSessionProvider.validatorCurrentFrame->status = LRP_RECEIVER_FRAME_READY_TO_CHECK;

    LRP_ReceiverValidatorLayer_handler(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].targetId, 0b11001);
    ASSERT_EQ(frameBuffer[0].command, 0b101);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, sourceDeviceId);
    ASSERT_EQ(frameBuffer[0].length, 3);
    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(*frameBuffer[0].data[1], data1);
    ASSERT_EQ(*frameBuffer[0].data[2], data2);
    ASSERT_EQ(frameBuffer[0].status, LRP_RECEIVER_FRAME_READY_TO_READ);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiverValidatorLayerTest,
       when_the_status_is_not_ready_to_check_should_be_handled_right) {
    receiverSessionProvider.validatorCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverValidatorLayer_handler(&receiverSessionProvider);

    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame->targetId, 0);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame->command, 0);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame->length, 0);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame->data[0], nullptr);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame->data[1], nullptr);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame->data[2], nullptr);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame->status, LRP_RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
}