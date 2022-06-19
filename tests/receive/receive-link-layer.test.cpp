#include <gtest/gtest.h>
#include "../../src/receive/receive-link-layer.h"
#include "../../src/receive/receive-session-provider.h"

class ReceiveLinkLayerTest : public ::testing::Test {
protected:

    LRPReceiveSessionProvider receiveSessionProvider{};
    const unsigned char sourceDeviceId = 0b10100u;
    const unsigned char groupId = 0b11100u;
    const unsigned char targetId = 0b11001u;
    const unsigned char command = 0b101u;
    const unsigned char length = 0b011u;

    LRPFrame frameBuffer[3]{};
    unsigned char receivedHeader0 = (unsigned char) (sourceDeviceId << 3u) | command;
    unsigned char receivedHeader1 = (unsigned char) (targetId << 3u) | length;
    unsigned char receivedData0 = 'L';
    unsigned char receivedData1 = 'R';
    unsigned char receivedData2 = 'P';

    void SetUp() override {
        LRP_ReceiveSessionProvider_init(&receiveSessionProvider, &sourceDeviceId, frameBuffer, 3, &groupId);
    }

    void TearDown() override {
        ASSERT_EQ(receiveSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*receiveSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(receiveSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(receiveSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(receiveSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Is_A_Given_Device) {
    receiveSessionProvider.indexOfReadBytes = 0;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData2);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], receivedData2);

    ASSERT_EQ(frameBuffer[0].targetId, 0);
    ASSERT_EQ(frameBuffer[0].command, 0);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, 0);
    ASSERT_EQ(frameBuffer[0].length, 0);
    ASSERT_EQ(frameBuffer[0].data[0], nullptr);
    ASSERT_EQ(frameBuffer[0].data[1], nullptr);
    ASSERT_EQ(frameBuffer[0].data[2], nullptr);
    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Id_Is_The_Broadcast_Id) {
    const unsigned char broadcastId = 0b11111u;
    const unsigned char header0WithBroadcastId =
            (unsigned char) (broadcastId << 3u) | receivedHeader0;

    receiveSessionProvider.indexOfReadBytes = 0;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData2);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], receivedData2);

    ASSERT_EQ(frameBuffer[0].targetId, 0);
    ASSERT_EQ(frameBuffer[0].command, 0);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, 0);
    ASSERT_EQ(frameBuffer[0].length, 0);
    ASSERT_EQ(frameBuffer[0].data[0], nullptr);
    ASSERT_EQ(frameBuffer[0].data[1], nullptr);
    ASSERT_EQ(frameBuffer[0].data[2], nullptr);
    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Id_Is_The_Group_Id_With_Is_Group_Flag) {
    const unsigned char header0WithGroupId =
            (unsigned char) (groupId << 3u) | FRAME_GROUP_ID_COMMAND;

    receiveSessionProvider.indexOfReadBytes = 0;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &receivedData2);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], receivedData2);

    ASSERT_EQ(frameBuffer[0].targetId, 0);
    ASSERT_EQ(frameBuffer[0].command, 0);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, 0);
    ASSERT_EQ(frameBuffer[0].length, 0);
    ASSERT_EQ(frameBuffer[0].data[0], nullptr);
    ASSERT_EQ(frameBuffer[0].data[1], nullptr);
    ASSERT_EQ(frameBuffer[0].data[2], nullptr);
    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Is_Not_That_Device) {
    const unsigned char anotherTargetId = 0b10101u;
    const unsigned char header0WithAnotherTargetId =
            (unsigned char) (anotherTargetId << 3u) | receivedHeader0;

    receiveSessionProvider.indexOfReadBytes = 0;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &header0WithAnotherTargetId);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].targetId, 0);
    ASSERT_EQ(frameBuffer[0].command, 0);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, 0);
    ASSERT_EQ(frameBuffer[0].length, 0);
    ASSERT_EQ(frameBuffer[0].data[0], nullptr);
    ASSERT_EQ(frameBuffer[0].data[1], nullptr);
    ASSERT_EQ(frameBuffer[0].data[2], nullptr);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Is_The_Group_Id_Without_Is_Group_Flag) {
    const unsigned char header0WithGroupId =
            (unsigned char) (groupId << 3u) | receivedHeader0;

    receiveSessionProvider.indexOfReadBytes = 0;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].targetId, 0);
    ASSERT_EQ(frameBuffer[0].command, 0);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, 0);
    ASSERT_EQ(frameBuffer[0].length, 0);
    ASSERT_EQ(frameBuffer[0].data[0], nullptr);
    ASSERT_EQ(frameBuffer[0].data[1], nullptr);
    ASSERT_EQ(frameBuffer[0].data[2], nullptr);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}


TEST_F(ReceiveLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Is_Not_That_Device_With_Is_Group_Flag) {
    const unsigned char anotherTargetId = 0b10101u;
    const unsigned char header0WithGroupId =
            (unsigned char) (anotherTargetId << 3u) | FRAME_GROUP_ID_COMMAND;

    receiveSessionProvider.indexOfReadBytes = 0;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_handler(&receiveSessionProvider, &header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].targetId, 0);
    ASSERT_EQ(frameBuffer[0].command, 0);
    ASSERT_EQ(frameBuffer[0].sourceDeviceId, 0);
    ASSERT_EQ(frameBuffer[0].length, 0);
    ASSERT_EQ(frameBuffer[0].data[0], nullptr);
    ASSERT_EQ(frameBuffer[0].data[1], nullptr);
    ASSERT_EQ(frameBuffer[0].data[2], nullptr);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Tried_To_Start_Receving_When_The_Status_Is_Not_Ready_To_Redefine) {

    receiveSessionProvider.indexOfReadBytes = 5;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    const unsigned char status = LRP_ReceiveLinkLayer_isStartReceiving(&receiveSessionProvider);

    ASSERT_FALSE(status);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Tried_To_Start_Receving_When_There_Is_Frame_Error) {

    receiveSessionProvider.indexOfReadBytes = 5;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_ERROR;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_DECODE_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char status = LRP_ReceiveLinkLayer_isStartReceiving(&receiveSessionProvider);

    ASSERT_FALSE(status);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Tried_To_Start_Receving_When_There_Is_No_Frame_Error_And_The_Status_Is_Ready_To_Redefine) {

    receiveSessionProvider.indexOfReadBytes = 5;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char status = LRP_ReceiveLinkLayer_isStartReceiving(&receiveSessionProvider);

    ASSERT_TRUE(status);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Start_Receving_When_There_Is_No_Frame_Error_And_The_Status_Is_Ready_To_Redefine) {

    receiveSessionProvider.indexOfReadBytes = 5;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    LRP_ReceiveLinkLayer_startReceiving(&receiveSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiveSessionProvider.indexOfReadBytes, 0);
    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Stoped_Receiving) {

    receiveSessionProvider.indexOfReadBytes = 5;
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_stopReceiving(&receiveSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_READY_TO_CHECK);
    ASSERT_EQ(receiveSessionProvider.indexOfReadBytes, 5);
    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Handled_Error_Status_When_There_Is_No_Error) {

    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_errorStatusHandler(&receiveSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLinkLayerTest,
       Should_Be_Handled_Error_Status_When_There_Is_Error) {

    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_ERROR;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_DECODE_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    LRP_ReceiveLinkLayer_errorStatusHandler(&receiveSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}
