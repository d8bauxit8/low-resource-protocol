#include <gtest/gtest.h>
#include "../../src/receive/receive_application_layer.h"

unsigned char testControl() {
    return 1;
}

class ReceiveApplicationLayerTest : public ::testing::Test {
protected:

    LRPReceiveSessionProvider receiveSessionProvider{};
    const unsigned char deviceId = 0b10100;
    LRPFrame frameBuffer[3]{};
    unsigned char data0 = 'L';

    LRPReceiveFrameController controllers[1]{reinterpret_cast<LRPReceiveFrameController>(testControl)};
    const unsigned char receiveFrameControllerListLength = 1;

    void SetUp() override {
        LRP_SessionProvider_init((LRPSessionProvider *) &receiveSessionProvider, &deviceId, frameBuffer, 3);

        receiveSessionProvider.applicationCurrentFrame->data[0] = &data0;
    }

    void TearDown() override {
        ASSERT_EQ(receiveSessionProvider.deviceId, &deviceId);
        ASSERT_EQ(*receiveSessionProvider.deviceId, deviceId);

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
       Should_Be_Handled_When_The_Status_Is_Ready_To_Read) {
    receiveSessionProvider.applicationCurrentFrame->status = RECEIVE_FRAME_READY_TO_READ;

    LRP_ReceiveApplicationLayer_handler(&receiveSessionProvider, controllers, receiveFrameControllerListLength);

    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiveApplicationLayerTest,
       Should_Be_Handled_When_The_Status_Is_Not_Ready_To_Read) {
    receiveSessionProvider.applicationCurrentFrame->status = RECEIVE_FRAME_READY_TO_CHECK;

    LRP_ReceiveApplicationLayer_handler(&receiveSessionProvider, controllers, receiveFrameControllerListLength);

    ASSERT_EQ(*frameBuffer[0].data[0], data0);
    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_READY_TO_CHECK);
    ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}
