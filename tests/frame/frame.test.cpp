#include <gtest/gtest.h>
#include "frame.test.h"

class FrameTest : public ::testing::Test {
protected:
    _LRPFrame *frame;
    const unsigned char const targetDeviceId = 0b00001;
    const unsigned char const command = 0b001;
    const unsigned char const sourceDeviceId = 0b00010;
    const unsigned char const length = 0b001;

    virtual void SetUp() override {
        frame = TEST_createLRPFrameInWhichTheBufferIsFilled(targetDeviceId, command, sourceDeviceId, length);
    }
};

TEST_F(FrameTest, Should_Be_Got_Target_Device_Id) {
    const unsigned char const data = (targetDeviceId << 3) | command;
    const unsigned char const targetDeviceIdToBeRead = LRP_Frame_getTargetDeviceIdFromReceivedByte(&data);
    ASSERT_EQ(targetDeviceIdToBeRead, targetDeviceId);
}

TEST_F(FrameTest, Should_Be_Added_Header_Data_To_Frame_Data_From_Frame_Buffer) {
    LRP_Frame_addHeaderDataToFrameDataFromFrameBuffer(frame);

    _FrameData *frameData = ((_FrameData *) frame);

    ASSERT_EQ(frameData->targetDeviceId, targetDeviceId);
    ASSERT_EQ(frameData->command, command);
    ASSERT_EQ(frameData->sourceDeviceId, sourceDeviceId);
    ASSERT_EQ(frameData->length, length);
}