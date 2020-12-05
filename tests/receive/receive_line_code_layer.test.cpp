#include <gtest/gtest.h>
#include "../../src/receive/receive_line_code_layer.h"

class ReceiveLineCodeLayerTest : public ::testing::Test {
protected:

    LRPReceiveSessionProvider receiveSessionProvider{};
    const unsigned char sourceDeviceId = 0b10100;
    const unsigned char targetDeviceId = 0b11001;
    const unsigned char command = 0b101;
    const unsigned char length = 0b011;

    LRPLineCode4B5B lineCode4B5B{};
    unsigned char buffer[2]{};

    LRPFrame frameBuffer[3]{};
    unsigned char header0 = 0b10100101;
    unsigned char header1 = 0b11001011;
    unsigned char data0 = 'L';
    unsigned char data1 = 'R';
    unsigned char data2 = 'P';

    void SetUp() override {
        LRP_SessionProvider_init((LRPSessionProvider *) &receiveSessionProvider, &sourceDeviceId, frameBuffer, 3);

        receiveSessionProvider.indexOfReadBytes = 0;

        lineCode4B5B.index = 0;
        lineCode4B5B.buffer[0] = 0;
        lineCode4B5B.buffer[1] = 0;
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

TEST_F(ReceiveLineCodeLayerTest,
       Should_Be_Handled_When_The_Data_Is_A_Start_Delimiter_And_The_Frame_Is_Ready_To_Start_Receiving) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char startDelimiter = LINE_CODE_4B5B_START_DELIMITER_BYTE;

    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &startDelimiter);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);

    ASSERT_EQ(lineCode4B5B.index, 0);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0);

    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLineCodeLayerTest,
       Should_Be_Handled_When_The_Data_Is_A_Start_Delimiter_And_The_Frame_Is_Not_Ready_To_Start_Receiving_Because_Of_Error) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_ERROR;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_DECODE_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char startDelimiter = LINE_CODE_4B5B_START_DELIMITER_BYTE;

    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &startDelimiter);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(lineCode4B5B.index, 4);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0xFF);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0xFE);

    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_DECODE_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLineCodeLayerTest,
       Should_Be_Handled_When_The_Data_Is_A_Start_Delimiter_And_The_Frame_Is_Not_Ready_To_Start_Receiving_Because_The_State_Of_Frame_Is_Not_Ready_To_Redefine) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    const unsigned char startDelimiter = LINE_CODE_4B5B_START_DELIMITER_BYTE;

    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &startDelimiter);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);

    ASSERT_EQ(lineCode4B5B.index, 4);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0xFF);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0xFE);

    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLineCodeLayerTest,
       Should_Be_Handled_When_The_Data_Is_Not_A_Start_Delimiter_But_The_Link_Layer_Is_Ready_To_Start_Receiving) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char someData = 0b10101010;

    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &someData);
    ASSERT_EQ(frameBuffer[0].buffer[0], 0);
    ASSERT_EQ(frameBuffer[0].buffer[1], 0);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);
    ASSERT_EQ(frameBuffer[0].buffer[3], 0);
    ASSERT_EQ(frameBuffer[0].buffer[4], 0);

    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(lineCode4B5B.index, 4);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0xFF);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0xFE);

    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLineCodeLayerTest,
       Should_Be_Handled_When_The_Receiving_Was_Already_Started) {
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    // Header, Data - LRP
    const unsigned char data[] = {0xCB, 0x5E, 0xAD, 0x15, 0x5D, 0x7E, 0x1};

    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[0]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[1]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[2]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[3]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[4]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[5]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[6]);

    ASSERT_EQ(frameBuffer[0].buffer[0], header0);
    ASSERT_EQ(frameBuffer[0].buffer[1], header1);
    ASSERT_EQ(frameBuffer[0].buffer[2], data0);
    ASSERT_EQ(frameBuffer[0].buffer[3], data1);
    ASSERT_EQ(frameBuffer[0].buffer[4], data2);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);

    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLineCodeLayerTest,
       Should_Be_Handled_When_The_Receiving_Was_Already_Started_And_Get_A_Wrong_Byte) {
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    // Header, Wrong byte (01101 is an invalid 4B5B code)
    // 0. bit is the previous byte last bit, 1-5 bits are an invalid code, 6-7 bits dummy
    const unsigned char data[] = {0xCB, 0x5E, 0xAD, 0b01011010};

    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[0]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[1]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[2]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[3]);

    ASSERT_EQ(frameBuffer[0].buffer[0], header0);
    ASSERT_EQ(frameBuffer[0].buffer[1], header1);
    ASSERT_EQ(frameBuffer[0].buffer[2], 0);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_IN_RECEIVING);

    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_DECODE_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[0]);
}

TEST_F(ReceiveLineCodeLayerTest,
       Should_Be_Handled_When_The_Receiving_Was_Already_Started_And_Get_A_Stop_Delimiter_Byte) {
    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = RECEIVE_FRAME_IN_RECEIVING;

    const unsigned char stopDelimiter = LINE_CODE_4B5B_STOP_DELIMITER_BYTE;

    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &stopDelimiter);

    ASSERT_EQ(frameBuffer[0].buffer[0], 0);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_READY_TO_CHECK);

    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}


TEST_F(ReceiveLineCodeLayerTest,
       Should_Be_Handled_A_Whole_Right_Flow) {
    lineCode4B5B.index = 4;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    receiveSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    receiveSessionProvider.linkLayerErrorCode = LINK_LAYER_NO_ERROR;
    receiveSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;

    const unsigned char startDelimiter = LINE_CODE_4B5B_START_DELIMITER_BYTE;
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &startDelimiter);

    // Header, Data - LRP
    const unsigned char data[] = {0xCB, 0x5E, 0xAD, 0x15, 0x5D, 0x7E, 0x1};

    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[0]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[1]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[2]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[3]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[4]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[5]);
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &data[6]);

    const unsigned char stopDelimiter = LINE_CODE_4B5B_STOP_DELIMITER_BYTE;
    LRP_ReceiveLineCodeLayer_handler(&receiveSessionProvider, &lineCode4B5B, &stopDelimiter);

    ASSERT_EQ(frameBuffer[0].buffer[0], header0);
    ASSERT_EQ(frameBuffer[0].buffer[1], header1);
    ASSERT_EQ(frameBuffer[0].buffer[2], data0);
    ASSERT_EQ(frameBuffer[0].buffer[3], data1);
    ASSERT_EQ(frameBuffer[0].buffer[4], data2);

    ASSERT_EQ(frameBuffer[0].status, RECEIVE_FRAME_READY_TO_CHECK);

    ASSERT_EQ(receiveSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);
    ASSERT_EQ(receiveSessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);
    ASSERT_EQ(receiveSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}
