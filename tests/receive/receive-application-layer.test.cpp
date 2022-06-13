#include <gtest/gtest.h>
#include "../../src/receive/receive-application-layer.h"
#include "../../src/receive/receive-session-provider.h"

unsigned char ReceiveApplicationLayerTest_testControlsCalls;
unsigned char ReceiveApplicationLayerTest_testValue1;
unsigned char ReceiveApplicationLayerTest_testValue2;

unsigned char ReceiveApplicationLayerTest_testControl1(LRPFrameData *const frameData) {
    ReceiveApplicationLayerTest_testControlsCalls++;
    if (*frameData->data[0] == 'L') {
        ReceiveApplicationLayerTest_testValue1++;
        return 1;
    }
    return 0;
}

unsigned char ReceiveApplicationLayerTest_testControl2(LRPFrameData *const frameData) {
    ReceiveApplicationLayerTest_testControlsCalls++;
    if (*frameData->data[0] == 'R') {
        ReceiveApplicationLayerTest_testValue2++;
        return 1;
    }
    return 0;
}

class ReceiveApplicationLayerTest : public ::testing::Test {
protected:

    LRPReceiveSessionProvider receiveSessionProvider{};
    const unsigned char groupId = 0b11100u;
    const unsigned char sourceDeviceId = 0b10100;
    LRPFrame frameBuffer[3]{};
    unsigned char data0 = 'L';

    LRPReceiveFrameController controllers[2]{ReceiveApplicationLayerTest_testControl1,
                                             ReceiveApplicationLayerTest_testControl2};
    const unsigned char numberOfReceiveFrameControllers = 2;

    void SetUp() override {
        LRP_ReceiveSessionProvider_init(&receiveSessionProvider, &sourceDeviceId, frameBuffer, 3, &groupId);

        receiveSessionProvider.applicationCurrentFrame->data[0] = &data0;

        ReceiveApplicationLayerTest_testControlsCalls = 0;
        ReceiveApplicationLayerTest_testValue1 = 0;
        ReceiveApplicationLayerTest_testValue2 = 0;
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

    LRP_ReceiveApplicationLayer_handler(&receiveSessionProvider, controllers, numberOfReceiveFrameControllers);

    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(ReceiveApplicationLayerTest_testControlsCalls, 1);
    ASSERT_EQ(ReceiveApplicationLayerTest_testValue1, 1);
    ASSERT_EQ(ReceiveApplicationLayerTest_testValue2, 0);
    ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiveApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Ready_To_Read_And_There_Is_Not_Controller_For_The_Data) {
    unsigned char untrackedData = 'P';
    receiveSessionProvider.applicationCurrentFrame->data[0] = &untrackedData;
    receiveSessionProvider.applicationCurrentFrame->status = RECEIVE_FRAME_READY_TO_READ;

    LRP_ReceiveApplicationLayer_handler(&receiveSessionProvider, controllers, numberOfReceiveFrameControllers);

    ASSERT_EQ(*frameBuffer[0].data[0], untrackedData);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(ReceiveApplicationLayerTest_testControlsCalls, 2);
    ASSERT_EQ(ReceiveApplicationLayerTest_testValue1, 0);
    ASSERT_EQ(ReceiveApplicationLayerTest_testValue2, 0);
    ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiveApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Not_Ready_To_Read) {
    receiveSessionProvider.applicationCurrentFrame->status = RECEIVE_FRAME_READY_TO_CHECK;

    LRP_ReceiveApplicationLayer_handler(&receiveSessionProvider, controllers, numberOfReceiveFrameControllers);

    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_READY_TO_CHECK);
    ASSERT_EQ(ReceiveApplicationLayerTest_testControlsCalls, 0);
    ASSERT_EQ(ReceiveApplicationLayerTest_testValue1, 0);
    ASSERT_EQ(ReceiveApplicationLayerTest_testValue2, 0);
    ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}
