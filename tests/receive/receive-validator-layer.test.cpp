#include <gtest/gtest.h>
#include "../../src/receive/receive-validator-layer.h"
#include "../../src/receive/receive-session-provider.h"

class ReceiveValidatorLayerTest : public ::testing::Test {
protected:

    LRPReceiveSessionProvider receiveSessionProvider{};
    const unsigned char sourceDeviceId = 0b10100u;
    const unsigned char groupId = 0b11100u;
    const unsigned char targetId = 0b11001u;
    const unsigned char command = 0b101u;
    const unsigned char length = 0b011u;
    LRPFrame frameBuffer[3]{};
    unsigned char data0 = 'L';
    unsigned char data1 = 'R';
    unsigned char data2 = 'P';

    void SetUp() override {
        LRP_ReceiveSessionProvider_init(&receiveSessionProvider, &sourceDeviceId, frameBuffer, 3, &groupId);

        frameBuffer[0].buffer[0] = (unsigned char) (targetId << 3u) | command;
        frameBuffer[0].buffer[1] = (unsigned char) (sourceDeviceId << 3u) | length;
        frameBuffer[0].buffer[2] = data0;
        frameBuffer[0].buffer[3] = data1;
        frameBuffer[0].buffer[4] = data2;
    }

    void TearDown() override {
        ASSERT_EQ(receiveSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*receiveSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
        ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);

        ASSERT_EQ(receiveSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(ReceiveValidatorLayerTest,
       Should_Be_Handled_When_The_Status_Is_Ready_To_Check) {
    receiveSessionProvider.validatorCurrentFrame->status = RECEIVE_FRAME_READY_TO_CHECK;

    LRP_ReceiveValidatorLayer_handler(&receiveSessionProvider);

    ASSERT_EQ(frameBuffer[0].targetId, 0b11001);
    ASSERT_EQ(frameBuffer[0].command, 0b101);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, sourceDeviceId);
    ASSERT_EQ(frameBuffer[0].length, 3);
    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(*frameBuffer[0].data[1], data1);
    ASSERT_EQ(*frameBuffer[0].data[2], data2);
    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_READY_TO_READ);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiveValidatorLayerTest,
       Should_Be_Handled_When_The_Status_Is_Not_Ready_To_Check) {
    receiveSessionProvider.validatorCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveValidatorLayer_handler(&receiveSessionProvider);

    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame->targetId, 0);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame->command, 0);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame->sourceDeviceId, 0);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame->length, 0);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame->data[0], nullptr);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame->data[1], nullptr);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame->data[2], nullptr);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame->status, RECEIVE_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
}