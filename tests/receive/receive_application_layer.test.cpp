#include <gtest/gtest.h>
#include "../../src/receive/receive_application_layer.h"

unsigned char testControlsCalls;
unsigned char testValue1;
unsigned char testValue2;

unsigned char testControl1(FrameData *const frameData) {
    testControlsCalls++;
    if (*frameData->data[0] == 'L') {
        testValue1++;
        return 1;
    }
    return 0;
}

unsigned char testControl2(FrameData *const frameData) {
    testControlsCalls++;
    if (*frameData->data[0] == 'R') {
        testValue2++;
        return 1;
    }
    return 0;
}

class ReceiveApplicationLayerTest : public ::testing::Test {
protected:

    LRPReceiveSessionProvider receiveSessionProvider{};
    const unsigned char sourceDeviceId = 0b10100;
    LRPFrame frameBuffer[3]{};
    unsigned char data0 = 'L';

    LRPReceiveFrameController controllers[2]{testControl1, testControl2};
    const unsigned char receiveFrameControllerListLength = 2;

    void SetUp() override {
        LRP_SessionProvider_init((LRPSessionProvider *) &receiveSessionProvider, &sourceDeviceId, frameBuffer, 3);

        receiveSessionProvider.applicationCurrentFrame->data[0] = &data0;

        testControlsCalls = 0;
        testValue1 = 0;
        testValue2 = 0;
    }

    void TearDown() override {
        ASSERT_EQ(receiveSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*receiveSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
        ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);

        ASSERT_EQ(receiveSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(ReceiveApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Ready_To_Read_And_There_Is_Controller_For_The_Data) {
    receiveSessionProvider.applicationCurrentFrame->status = RECEIVE_FRAME_READY_TO_READ;

    LRP_ReceiveApplicationLayer_handler(&receiveSessionProvider, controllers, receiveFrameControllerListLength);

    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(testControlsCalls, 1);
    ASSERT_EQ(testValue1, 1);
    ASSERT_EQ(testValue2, 0);
    ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiveApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Ready_To_Read_And_There_Is_Not_Controller_For_The_Data) {
    unsigned char untrackedData = 'P';
    receiveSessionProvider.applicationCurrentFrame->data[0] = &untrackedData;
    receiveSessionProvider.applicationCurrentFrame->status = RECEIVE_FRAME_READY_TO_READ;

    LRP_ReceiveApplicationLayer_handler(&receiveSessionProvider, controllers, receiveFrameControllerListLength);

    ASSERT_EQ(*frameBuffer[0].data[0], untrackedData);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(testControlsCalls, 2);
    ASSERT_EQ(testValue1, 0);
    ASSERT_EQ(testValue2, 0);
    ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiveApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Not_Ready_To_Read) {
    receiveSessionProvider.applicationCurrentFrame->status = RECEIVE_FRAME_READY_TO_CHECK;

    LRP_ReceiveApplicationLayer_handler(&receiveSessionProvider, controllers, receiveFrameControllerListLength);

    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_READY_TO_CHECK);
    ASSERT_EQ(testControlsCalls, 0);
    ASSERT_EQ(testValue1, 0);
    ASSERT_EQ(testValue2, 0);
    ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}
