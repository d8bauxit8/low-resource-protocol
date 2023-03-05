#include <gtest/gtest.h>
#include "../src/frame.h"

class FrameTest : public ::testing::Test {
protected:
    LRPFrame frame{};

    const unsigned char targetId = 0b11001u;
    const unsigned char command = LRP_FRAME_NO_COMMAND;
    const unsigned char sourceDeviceId = 0b10100u;
    const unsigned char length = 3u;
    const unsigned char status = 0xFFu;
    unsigned char data0 = 'L';
    unsigned char data1 = 'R';
    unsigned char data2 = 'P';


    const unsigned char buffer0 = (unsigned char) (targetId << 3u) | command;
    const unsigned char buffer1 = (unsigned char) (sourceDeviceId << 3u) | length;
    const unsigned char buffer2 = data0;
    const unsigned char buffer3 = data1;
    const unsigned char buffer4 = data2;

    void SetUp() override {
        frame.status = status;
    }

    void initDataPart() {
        frame.targetId = targetId;
        frame.command = command;
        frame.sourceDeviceId = sourceDeviceId;
        frame.length = length;
        frame.status = status;
        frame.data[0] = &data0;
        frame.data[1] = &data1;
        frame.data[2] = &data2;
    }

    void initBufferPart() {
        frame.buffer[0] = (unsigned char) (targetId << 3u) | command;
        frame.buffer[1] = (unsigned char) (sourceDeviceId << 3u) | length;
        frame.buffer[2] = buffer2;
        frame.buffer[3] = buffer3;
        frame.buffer[4] = buffer4;
    }
};

TEST_F(FrameTest, when_the_buffer_contains_one_item_should_be_initialized_right) {
    const unsigned char frameBufferLength = 1;
    LRPFrame frameBuffer[1]{};

    LRP_Frame_initBuffer(frameBuffer, &frameBufferLength);
    ASSERT_EQ(&frameBuffer[0], frameBuffer[0].next);
    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
}

TEST_F(FrameTest, when_the_buffer_contains_three_item_should_be_initialized_right) {
    const unsigned char frameBufferLength = 3u;
    LRPFrame frameBuffer[3]{};

    LRP_Frame_initBuffer(frameBuffer, &frameBufferLength);

    ASSERT_EQ(&frameBuffer[0], frameBuffer[2].next);
    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(&frameBuffer[1], frameBuffer[0].next);
    ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(&frameBuffer[2], frameBuffer[1].next);
    ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);
}

TEST_F(FrameTest, status_should_be_set_right) {
    const unsigned char uniqueStatus = 0b00011100u;
    LRP_Frame_setStatus(&frame, uniqueStatus);
    ASSERT_EQ(frame.status, uniqueStatus);
}

TEST_F(FrameTest, status_should_be_reseted_right) {
    LRP_Frame_resetStatus(&frame);
    ASSERT_EQ(frame.status, LRP_FRAME_READY_TO_REDEFINE);
}

TEST_F(FrameTest, target_device_id_should_be_got_right) {
    initBufferPart();

    const unsigned char targetIdToBeRead = LRP_Frame_getTargetIdFromReceivedByte(&frame.buffer[0]);
    ASSERT_EQ(targetIdToBeRead, targetId);
}

TEST_F(FrameTest, when_data_contains_group_id_isGroupIdCommandFromReceivedByte_should_return_true) {
    const unsigned char data = 0b00000111u;
    const unsigned char isGroupId = LRP_Frame_isGroupIdCommandFromReceivedByte(&data);
    ASSERT_TRUE(isGroupId);
}

TEST_F(FrameTest, when_data_does_not_contain_group_id_isGroupIdCommandFromReceivedByte_should_return_false) {
    const unsigned char data = 0b00000000u;
    const unsigned char isGroupId = LRP_Frame_isGroupIdCommandFromReceivedByte(&data);
    ASSERT_FALSE(isGroupId);
}

TEST_F(FrameTest, header_data_should_be_added_to_frame_data_from_frame_buffer) {
    initBufferPart();

    LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(&frame);

    LRPFrameData *frameData = ((LRPFrameData *) &frame);

    ASSERT_EQ(frameData->targetId, targetId);
    ASSERT_EQ(frameData->command, command);
    ASSERT_EQ(frameData->sourceDeviceId, sourceDeviceId);
    ASSERT_EQ(frameData->length, length);
}

TEST_F(FrameTest, data_should_be_added_to_frame_data_from_frame_buffer) {
    frame.length = length;
    initBufferPart();

    LRP_Frame_addDataToFrameDataFromFrameBuffer(&frame);

    LRPFrameData *frameData = ((LRPFrameData *) &frame);

    ASSERT_EQ(*frameData->data[0], buffer2);
    ASSERT_EQ(*frameData->data[1], buffer3);
}

TEST_F(FrameTest, header_data_should_be_added_to_frame_buffer_from_frame_data) {
    initDataPart();

    LRP_Frame_addHeaderDataToFrameBufferFromFrameData(&frame);

    ASSERT_EQ(frame.buffer[0], buffer0);
    ASSERT_EQ(frame.buffer[1], buffer1);
}

TEST_F(FrameTest, data_should_be_added_to_frame_buffer_from_frame_data) {
    initDataPart();

    LRP_Frame_addDataToFrameBufferFromFrameData(&frame);

    ASSERT_EQ(frame.buffer[2], buffer2);
    ASSERT_EQ(frame.buffer[3], buffer3);
}

