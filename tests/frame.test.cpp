#include <gtest/gtest.h>
#include "../src/frame.h"

class FrameTest : public ::testing::Test {
protected:
    LRPFrame frame{};

    const unsigned char targetDeviceId = 0b00001;
    const unsigned char command = 0b001;
    const unsigned char sourceDeviceId = 0b00010;
    const unsigned char length = 0b010;
    const unsigned char status = 0xFF;
    unsigned char data0 = 0b00011000;
    unsigned char data1 = 0b11100111;


    const unsigned char buffer0 = (unsigned char) (targetDeviceId << 3u) | command;
    const unsigned char buffer1 = (unsigned char) (sourceDeviceId << 3u) | length;
    const unsigned char buffer2 = data0;
    const unsigned char buffer3 = data1;

    void SetUp() override {
        frame.status = status;
    }

    void initDataPart() {
        frame.targetDeviceId = targetDeviceId;
        frame.command = command;
        frame.sourceDeviceId = sourceDeviceId;
        frame.length = length;
        frame.status = status;
        frame.data[0] = &data0;
        frame.data[1] = &data1;
    }

    void initBufferPart() {
        frame.buffer[0] = (unsigned char) (targetDeviceId << 3u) | command;
        frame.buffer[1] = (unsigned char) (sourceDeviceId << 3u) | length;
        frame.buffer[2] = buffer2;
        frame.buffer[3] = buffer3;
    }
};

TEST_F(FrameTest, Should_Be_Initialized_Buffer_When_The_Buffer_Contains_One_Item) {
    const unsigned char frameBufferLength = 1;
    LRPFrame frameBuffer[1]{};

    LRP_Frame_initBuffer(frameBuffer, &frameBufferLength);
    ASSERT_EQ(&frameBuffer[0], frameBuffer[0].next);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
}

TEST_F(FrameTest, Should_Be_Initialized_Buffer_When_The_Buffer_Contains_Three_Items) {
    const unsigned char frameBufferLength = 3;
    LRPFrame frameBuffer[3]{};

    LRP_Frame_initBuffer(frameBuffer, &frameBufferLength);

    ASSERT_EQ(&frameBuffer[0], frameBuffer[2].next);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(&frameBuffer[1], frameBuffer[0].next);
    ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(&frameBuffer[2], frameBuffer[1].next);
    ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);
}

TEST_F(FrameTest, Should_Be_Set_Status) {
    const unsigned char uniqueStatus = 0b00011100;
    LRP_Frame_setStatus(&frame, uniqueStatus);
    ASSERT_EQ(frame.status, uniqueStatus);
}

TEST_F(FrameTest, Should_Be_Reseted_Status) {
    LRP_Frame_resetStatus(&frame);
    ASSERT_EQ(frame.status, FRAME_READY_TO_REDEFINE);
}

TEST_F(FrameTest, Should_Be_Got_Target_Device_Id) {
    initBufferPart();

    const unsigned char targetDeviceIdToBeRead = LRP_Frame_getTargetDeviceIdFromReceivedByte(&frame.buffer[0]);
    ASSERT_EQ(targetDeviceIdToBeRead, targetDeviceId);
}

TEST_F(FrameTest, Should_Be_Added_Header_Data_To_Frame_Data_From_Frame_Buffer) {
    initBufferPart();

    LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(&frame);

    FrameData *frameData = ((FrameData *) &frame);

    ASSERT_EQ(frameData->targetDeviceId, targetDeviceId);
    ASSERT_EQ(frameData->command, command);
    ASSERT_EQ(frameData->sourceDeviceId, sourceDeviceId);
    ASSERT_EQ(frameData->length, length);
}

TEST_F(FrameTest, Should_Be_Added_Data_To_Frame_Data_From_Frame_Buffer) {
    frame.length = length;
    initBufferPart();

    LRP_Frame_addDataToFrameDataFromFrameBuffer(&frame);

    FrameData *frameData = ((FrameData *) &frame);

    ASSERT_EQ(*frameData->data[0], buffer2);
    ASSERT_EQ(*frameData->data[1], buffer3);
}

TEST_F(FrameTest, Should_Be_Added_Header_Data_To_Frame_Buffer_From_Frame_Data) {
    initDataPart();

    LRP_Frame_addHeaderDataToFrameBufferFromFrameData(&frame);

    ASSERT_EQ(frame.buffer[0], buffer0);
    ASSERT_EQ(frame.buffer[1], buffer1);
}

TEST_F(FrameTest, Should_Be_Added_Data_To_Frame_Buffer_From_Frame_Data) {
    initDataPart();

    LRP_Frame_addDataToFrameBufferFromFrameData(&frame);

    ASSERT_EQ(frame.buffer[2], buffer2);
    ASSERT_EQ(frame.buffer[3], buffer3);
}

