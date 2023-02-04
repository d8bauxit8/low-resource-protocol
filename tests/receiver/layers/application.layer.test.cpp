#include <gtest/gtest.h>
#include "../../../src/receiver/layers/application.layer.h"
#include "../../../src/receiver/receiver.module.h"

unsigned char ReceiverApplicationLayerTest_testControlsCalls;
unsigned char ReceiverApplicationLayerTest_testValue1;
unsigned char ReceiverApplicationLayerTest_testValue2;

unsigned char ReceiverApplicationLayerTest_testControl1(const LRPFrameData *const frameData) {
    ReceiverApplicationLayerTest_testControlsCalls++;
    if (*frameData->data[0] == 'L') {
        ReceiverApplicationLayerTest_testValue1++;
        return 1;
    }
    return 0;
}

unsigned char ReceiverApplicationLayerTest_testControl2(const LRPFrameData *const frameData) {
    ReceiverApplicationLayerTest_testControlsCalls++;
    if (*frameData->data[0] == 'R') {
        ReceiverApplicationLayerTest_testValue2++;
        return 1;
    }
    return 0;
}

class ReceiverApplicationLayerTest : public ::testing::Test {
protected:

    LRPReceiverSessionProvider receiverSessionProvider{};
    unsigned char groupId = 0b11100u;
    unsigned char sourceDeviceId = 0b10100;
    LRPFrame frameBuffer[3]{};
    unsigned char data0 = 'L';

    const LRPReceiverFrameController controllers[2]{ReceiverApplicationLayerTest_testControl1,
                                                    ReceiverApplicationLayerTest_testControl2};
    const unsigned char numberOfReceiverFrameControllers = 2;

    void SetUp() override {
        LRP_Receiver_init(&receiverSessionProvider, &sourceDeviceId, frameBuffer, 3, &groupId);

        receiverSessionProvider.applicationCurrentFrame->data[0] = &data0;

        ReceiverApplicationLayerTest_testControlsCalls = 0;
        ReceiverApplicationLayerTest_testValue1 = 0;
        ReceiverApplicationLayerTest_testValue2 = 0;
    }

    void TearDown() override {
        ASSERT_EQ(receiverSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*receiverSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
        ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);

        ASSERT_EQ(receiverSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(ReceiverApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Ready_To_Read_And_There_Is_Controller_For_The_Data) {
    receiverSessionProvider.applicationCurrentFrame->status = LRP_RECEIVER_FRAME_READY_TO_READ;

    LRP_ReceiverApplicationLayer_handler(&receiverSessionProvider, controllers, numberOfReceiverFrameControllers);

    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(ReceiverApplicationLayerTest_testControlsCalls, 1);
    ASSERT_EQ(ReceiverApplicationLayerTest_testValue1, 1);
    ASSERT_EQ(ReceiverApplicationLayerTest_testValue2, 0);
    ASSERT_EQ(receiverSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiverApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Ready_To_Read_And_There_Is_Not_Controller_For_The_Data) {
    unsigned char untrackedData = 'P';
    receiverSessionProvider.applicationCurrentFrame->data[0] = &untrackedData;
    receiverSessionProvider.applicationCurrentFrame->status = LRP_RECEIVER_FRAME_READY_TO_READ;

    LRP_ReceiverApplicationLayer_handler(&receiverSessionProvider, controllers, numberOfReceiverFrameControllers);

    ASSERT_EQ(*frameBuffer[0].data[0], untrackedData);
    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(ReceiverApplicationLayerTest_testControlsCalls, 2);
    ASSERT_EQ(ReceiverApplicationLayerTest_testValue1, 0);
    ASSERT_EQ(ReceiverApplicationLayerTest_testValue2, 0);
    ASSERT_EQ(receiverSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiverApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Not_Ready_To_Read) {
    receiverSessionProvider.applicationCurrentFrame->status = LRP_RECEIVER_FRAME_READY_TO_CHECK;

    LRP_ReceiverApplicationLayer_handler(&receiverSessionProvider, controllers, numberOfReceiverFrameControllers);

    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(frameBuffer[0].status, LRP_RECEIVER_FRAME_READY_TO_CHECK);
    ASSERT_EQ(ReceiverApplicationLayerTest_testControlsCalls, 0);
    ASSERT_EQ(ReceiverApplicationLayerTest_testValue1, 0);
    ASSERT_EQ(ReceiverApplicationLayerTest_testValue2, 0);
    ASSERT_EQ(receiverSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}
