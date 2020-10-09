#include <gtest/gtest.h>
#include "../src/line_code_4b5b.h"

class LineCode4b5bTest : public ::testing::Test {
protected:

    _LRPLineCode4B5B lineCode4B5B{};
    unsigned char buffer[2]{};

    void SetUp() override {
        lineCode4B5B.index = 0;
        lineCode4B5B.buffer[0] = &buffer[0];
        lineCode4B5B.buffer[1] = &buffer[1];
    }
};

TEST_F(LineCode4b5bTest, Should_Be_Reseted) {
    lineCode4B5B.index = 2;

    LRP_4B5B_reset(&lineCode4B5B);

    ASSERT_EQ(lineCode4B5B.index, 0);
}

TEST_F(LineCode4b5bTest, Buffer_Of_Encoded_Bits_Should_Be_Ready_To_Read_A_Decoded_Byte) {
    lineCode4B5B.index = 0;
    ASSERT_EQ(LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(&lineCode4B5B), 0);

    lineCode4B5B.index = 1;
    ASSERT_EQ(LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(&lineCode4B5B), 0);

    lineCode4B5B.index = 10;
    ASSERT_EQ(LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(&lineCode4B5B), 1);

    lineCode4B5B.index = 11;
    ASSERT_EQ(LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(&lineCode4B5B), 1);
}

TEST_F(LineCode4b5bTest, Should_Be_Added_Encoded_Byte_To_Buffer_Of_Ecoded_Bits) {
    lineCode4B5B.index = 2;

    LRP_4B5B_reset(&lineCode4B5B);

    ASSERT_EQ(lineCode4B5B.index, 0);
}
