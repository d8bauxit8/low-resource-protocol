#include <gtest/gtest.h>
#include "../../../src/receiver/layers/line-code.layer.h"
#include "../../../src/receiver/receiver.module.h"

class ReceiverLineCodeLayerTest : public ::testing::Test {
protected:

    LRPReceiverSessionProvider receiverSessionProvider{};
    unsigned char sourceDeviceId = 0b10100u;
    unsigned char groupId = 0b11100u;
    const unsigned char targetId = 0b11001u;
    const unsigned char command = 0b101u;
    const unsigned char length = 0b011u;

    LRPLineCode4B5B lineCode4B5B{};

    LRPFrame frameBuffer[3]{};
    unsigned char receivedHeader0 = (unsigned char) (sourceDeviceId << 3u) | command;
    unsigned char receivedHeader1 = (unsigned char) (targetId << 3u) | length;
    unsigned char receivedData0 = 'L';
    unsigned char receivedData1 = 'R';
    unsigned char receivedData2 = 'P';

    void SetUp() override {
        LRP_Receiver_init(&receiverSessionProvider, &sourceDeviceId, frameBuffer, 3, &groupId);

        receiverSessionProvider.indexOfReadBytes = 0;

        lineCode4B5B.index = 0;
        lineCode4B5B.buffer[0] = 0;
        lineCode4B5B.buffer[1] = 0;
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

TEST_F(ReceiverLineCodeLayerTest,
       Should_Be_Handled_When_The_Data_Is_A_Start_Delimiter_And_The_Frame_Is_Ready_To_Start_Receiving) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiverSessionProvider.linkLayerStatus = Skip;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char startDelimiter = LINE_CODE_4B5B_START_DELIMITER_BYTE;

    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &startDelimiter);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);

    ASSERT_EQ(lineCode4B5B.index, 0);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0);

    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, OK);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLineCodeLayerTest,
       Should_Be_Handled_When_The_Data_Is_A_Start_Delimiter_And_The_Frame_Is_Not_Ready_To_Start_Receiving_Because_Of_Error) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiverSessionProvider.linkLayerStatus = Error;
    receiverSessionProvider.linkLayerErrorCode = DecodeError;
    receiverSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char startDelimiter = LINE_CODE_4B5B_START_DELIMITER_BYTE;

    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &startDelimiter);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(lineCode4B5B.index, 4);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0xFF);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0xFE);

    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Error);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, DecodeError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLineCodeLayerTest,
       Should_Be_Handled_When_The_Data_Is_A_Start_Delimiter_And_The_Frame_Is_Not_Ready_To_Start_Receiving_Because_The_State_Of_Frame_Is_Not_Ready_To_Redefine) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiverSessionProvider.linkLayerStatus = Skip;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    const unsigned char startDelimiter = LINE_CODE_4B5B_START_DELIMITER_BYTE;

    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &startDelimiter);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);

    ASSERT_EQ(lineCode4B5B.index, 4);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0xFF);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0xFE);

    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLineCodeLayerTest,
       Should_Be_Handled_When_The_Data_Is_Not_A_Start_Delimiter_But_The_Link_Layer_Is_Ready_To_Start_Receiving) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiverSessionProvider.linkLayerStatus = Skip;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char someData = 0b10101010;

    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &someData);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(lineCode4B5B.index, 4);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0xFF);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0xFE);

    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLineCodeLayerTest,
       Should_Be_Handled_When_The_Receiving_Was_Already_Started) {
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    // Header, Data - LRP
    const unsigned char data[] = {0xCB, 0x5E, 0xAD, 0x15, 0x5D, 0x7E, 0x1};

    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[0]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[1]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[2]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[3]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[4]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[5]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[6]);

    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], receivedData2);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);

    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, OK);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLineCodeLayerTest,
       Should_Be_Handled_When_The_Receiving_Was_Already_Started_And_Get_A_Wrong_Byte) {
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    // Header, Wrong byte (01101 is an invalid 4B5B code)
    // 0. bit is the previous byte last bit, 1-5 bits are an invalid code, 6-7 bits dummy
    const unsigned char data[] = {0xCB, 0x5E, 0xAD, 0b01011010};

    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[0]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[1]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[2]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[3]);

    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_IN_RECEIVING);

    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Error);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, DecodeError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiverLineCodeLayerTest,
       Should_Be_Handled_When_The_Receiving_Was_Already_Started_And_Get_A_Stop_Delimiter_Byte) {
    receiverSessionProvider.linkLayerStatus = OK;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = RECEIVER_FRAME_IN_RECEIVING;

    const unsigned char stopDelimiter = LINE_CODE_4B5B_STOP_DELIMITER_BYTE;

    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &stopDelimiter);

    ASSERT_EQ(frameBuffer[0].buffer[0], 0);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_READY_TO_CHECK);

    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}


TEST_F(ReceiverLineCodeLayerTest,
       Should_Be_Handled_A_Whole_Right_Flow) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiverSessionProvider.linkLayerStatus = Skip;
    receiverSessionProvider.linkLayerErrorCode = NoError;
    receiverSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char startDelimiter = LINE_CODE_4B5B_START_DELIMITER_BYTE;
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &startDelimiter);

    // Header, Data - LRP
    const unsigned char data[] = {0xCB, 0x5E, 0xAD, 0x15, 0x5D, 0x7E, 0x1};

    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[0]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[1]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[2]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[3]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[4]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[5]);
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &data[6]);

    const unsigned char stopDelimiter = LINE_CODE_4B5B_STOP_DELIMITER_BYTE;
    LRP_ReceiverLineCodeLayer_handler(&receiverSessionProvider, &lineCode4B5B, &stopDelimiter);

    ASSERT_EQ(frameBuffer[0].buffer[0], receivedHeader0);
    ASSERT_EQ(frameBuffer[0].buffer[1], receivedHeader1);
    ASSERT_EQ(frameBuffer[0].buffer[2], receivedData0);
    ASSERT_EQ(frameBuffer[0].buffer[3], receivedData1);
    ASSERT_EQ(frameBuffer[0].buffer[4], receivedData2);

    ASSERT_EQ(frameBuffer[0].status, RECEIVER_FRAME_READY_TO_CHECK);

    ASSERT_EQ(receiverSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(receiverSessionProvider.linkLayerErrorCode, NoError);
    ASSERT_EQ(receiverSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}
