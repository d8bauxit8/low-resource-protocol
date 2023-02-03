#include <gtest/gtest.h>
#include "../../../src/receiver/layers/link.layer.h"
#include "../../../src/receiver/receiver.module.h"

class ReceiverLinkLayerTest : public ::testing::Test {
protected:

    LRPReceiverSessionProvider receiverSessionProvider{};
    unsigned char sourceDeviceId = 0b10100u;
    unsigned char groupId = 0b11100u;
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
        LRP_Receiver_init(&receiverSessionProvider, &sourceDeviceId, frameBuffer, 3, &groupId);
    }

    void TearDown() override {
        ASSERT_EQ(receiverSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*receiverSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(receiverSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(receiverSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Is_A_Given_Device) {
    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData2);
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
    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Id_Is_The_Broadcast_Id) {
    const unsigned char broadcastId = 0b11111u;
    const unsigned char header0WithBroadcastId =
            (unsigned char) (broadcastId << 3u) | receivedHeader0;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithBroadcastId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData2);
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
    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Id_Is_The_Group_Id_With_Is_Group_Flag) {
    const unsigned char header0WithGroupId =
            (unsigned char) (groupId << 3u) | GroupIdCommand;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[0], header0WithGroupId);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &receivedData2);
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
    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Is_Not_That_Device) {
    const unsigned char anotherTargetId = 0b10101u;
    const unsigned char header0WithAnotherTargetId =
            (unsigned char) (anotherTargetId << 3u) | receivedHeader0;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &header0WithAnotherTargetId);
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
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Is_The_Group_Id_Without_Is_Group_Flag) {
    const unsigned char header0WithGroupId =
            (unsigned char) (groupId << 3u) | receivedHeader0;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &header0WithGroupId);
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
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}


TEST_F(ReceiverLinkLayerTest,
       Should_Be_Handled_When_The_Status_Is_Receiving_And_The_Target_Device_Is_Not_That_Device_With_Is_Group_Flag) {
    const unsigned char anotherTargetId = 0b10101u;
    const unsigned char header0WithGroupId =
            (unsigned char) (anotherTargetId << 3u) | GroupIdCommand;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_handler(&receiverSessionProvider, &header0WithGroupId);
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
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Tried_To_Start_Receving_When_The_Status_Is_Not_Ready_To_Redefine) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = Skip;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    const unsigned char status = LRP_ReceiverLinkLayer_isStartReceiving(&receiverSessionProvider);

    ASSERT_FALSE(status);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Tried_To_Start_Receving_When_There_Is_Frame_Error) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = Error;
    receiverSessionProvider.linkLayerErrorCode = DecodeError;
    receiverSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char status = LRP_ReceiverLinkLayer_isStartReceiving(&receiverSessionProvider);

    ASSERT_FALSE(status);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Tried_To_Start_Receving_When_There_Is_No_Frame_Error_And_The_Status_Is_Ready_To_Redefine) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = Skip;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char status = LRP_ReceiverLinkLayer_isStartReceiving(&receiverSessionProvider);

    ASSERT_TRUE(status);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Start_Receving_When_There_Is_No_Frame_Error_And_The_Status_Is_Ready_To_Redefine) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = Skip;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    LRP_ReceiverLinkLayer_startReceiving(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.indexOfReadBytes, 0);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, OK);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Stoped_Receiving) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_stopReceiving(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_READY_TO_CHECK);
    ASSERT_EQ(receiverSessionProvider.indexOfReadBytes, 5);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Handled_Error_Status_When_There_Is_No_Error) {

    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_errorStatusHandler(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, OK);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       Should_Be_Handled_Error_Status_When_There_Is_Error) {

    receiverSessionProvider.linkLayerStatus = Error;
    receiverSessionProvider.linkLayerErrorCode = DecodeError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_errorStatusHandler(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}
