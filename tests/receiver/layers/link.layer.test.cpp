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

        ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(receiverSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(receiverSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(ReceiverLinkLayerTest,
       when_the_status_is_receiving_and_the_target_device_is_a_given_device_id_should_be_handled_right) {
    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

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
    ASSERT_EQ(frameBuffer[0].status, LRP_RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       when_the_status_is_receiving_and_the_target_device_is_the_broadcast_id_should_be_handled_right) {
    const unsigned char broadcastId = 0b11111u;
    const unsigned char header0WithBroadcastId =
            (unsigned char) (broadcastId << 3u) | receivedHeader0;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

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
    ASSERT_EQ(frameBuffer[0].status, LRP_RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       when_the_status_is_receiving_and_the_target_device_is_a_given_group_id_and_group_flag_exists_should_be_handled_right) {
    const unsigned char header0WithGroupId =
            (unsigned char) (groupId << 3u) | LRP_FRAME_GROUP_ID_COMMAND;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

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
    ASSERT_EQ(frameBuffer[0].status, LRP_RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       when_the_status_is_receiving_and_the_target_device_is_another_device_id_should_be_handled_right) {
    const unsigned char anotherTargetId = 0b10101u;
    const unsigned char header0WithAnotherTargetId =
            (unsigned char) (anotherTargetId << 3u) | receivedHeader0;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

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
    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       when_the_status_is_receiving_and_the_target_device_is_a_given_group_id_and_group_flag_does_not_exist_should_be_handled_right) {
    const unsigned char header0WithGroupId =
            (unsigned char) (groupId << 3u) | receivedHeader0;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

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
    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}


TEST_F(ReceiverLinkLayerTest,
       when_the_status_is_receiving_and_the_target_device_is_another_device_id_and_group_flag_exists_should_be_handled_right) {
    const unsigned char anotherTargetId = 0b10101u;
    const unsigned char header0WithGroupId =
            (unsigned char) (anotherTargetId << 3u) | LRP_FRAME_GROUP_ID_COMMAND;

    receiverSessionProvider.indexOfReadBytes = 0;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

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
    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       when_the_status_is_not_ready_to_redefine_should_be_tried_to_start_receving) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_SKIP;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

    const unsigned char status = LRP_ReceiverLinkLayer_isStartReceiving(&receiverSessionProvider);

    ASSERT_FALSE(status);
}

TEST_F(ReceiverLinkLayerTest,
       when_frame_has_an_error_should_be_tried_to_start_receving) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_ERROR;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_DECODE_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_FRAME_READY_TO_REDEFINE;

    const unsigned char status = LRP_ReceiverLinkLayer_isStartReceiving(&receiverSessionProvider);

    ASSERT_FALSE(status);
}

TEST_F(ReceiverLinkLayerTest,
       when_frame_does_not_have_error_and_the_status_is_ready_to_redefine_should_be_tried_to_start_receiving) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_SKIP;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_FRAME_READY_TO_REDEFINE;

    const unsigned char status = LRP_ReceiverLinkLayer_isStartReceiving(&receiverSessionProvider);

    ASSERT_TRUE(status);
}

TEST_F(ReceiverLinkLayerTest,
       when_frame_has_error_and_the_status_is_ready_to_redefine_should_be_tried_to_start_receiving) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_SKIP;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_FRAME_READY_TO_REDEFINE;

    LRP_ReceiverLinkLayer_startReceiving(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, LRP_RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.indexOfReadBytes, 0);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_OK);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       receiving_should_be_stopped) {

    receiverSessionProvider.indexOfReadBytes = 5;
    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_stopReceiving(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, LRP_RECEIVER_FRAME_READY_TO_CHECK);
    ASSERT_EQ(receiverSessionProvider.indexOfReadBytes, 5);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}

TEST_F(ReceiverLinkLayerTest,
       when_layer_does_not_have_error_should_be_handled_right) {

    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_errorStatusHandler(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, LRP_RECEIVER_FRAME_IN_RECEIVING);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_OK);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLinkLayerTest,
       when_layer_has_error_should_be_handled_right) {

    receiverSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_ERROR;
    receiverSessionProvider.linkLayerErrorCode = LRP_LINK_LAYER_DECODE_ERROR;
    receiverSessionProvider.linkCurrentFrame->status = LRP_RECEIVER_FRAME_IN_RECEIVING;

    LRP_ReceiverLinkLayer_errorStatusHandler(&receiverSessionProvider);

    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}
