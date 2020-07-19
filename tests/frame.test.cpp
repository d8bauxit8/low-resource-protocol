#include <gtest/gtest.h>
#include "../src/frame.h"

class FrameTest : public ::testing::Test {
protected:
    _LRPFrame frame{};
    const unsigned char const targetDeviceId = 20;
    const unsigned char const command = 2;
};

TEST_F(FrameTest, Should_Be_Set_Status) {
    LRP_Frame_setStatus(&frame, FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(frame.status, FRAME_READY_TO_REDEFINE);
}

TEST_F(FrameTest, Should_Be_Reset_Status) {
    LRP_Frame_resetStatus(&frame);

    ASSERT_EQ(frame.status, FRAME_READY_TO_REDEFINE);
}

TEST_F(FrameTest, Should_Be_Got_Target_Device_Id) {
    const unsigned char const data = (targetDeviceId << 3) | command;
    const unsigned char const targetDeviceIdToBeRead = LRP_Frame_getTargetDeviceIdFromReceivedByte(&data);
    ASSERT_EQ(targetDeviceIdToBeRead, targetDeviceId);
}
