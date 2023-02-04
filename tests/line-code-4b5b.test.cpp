#include <gtest/gtest.h>
#include "../src/line-code-4b5b.h"

class LineCode4b5bTest : public ::testing::Test {
protected:

    LRPLineCode4B5B lineCode4B5B{};

    void SetUp() override {
        lineCode4B5B.index = 0u;
        lineCode4B5B.buffer[0] = 0u;
        lineCode4B5B.buffer[1] = 0u;
    }
};

TEST_F(LineCode4b5bTest, Should_Be_Reseted) {
    lineCode4B5B.index = 2u;
    lineCode4B5B.buffer[0] = 0xFFu;
    lineCode4B5B.buffer[1] = 0xFEu;

    LRP_4B5B_reset(&lineCode4B5B);

    ASSERT_EQ(lineCode4B5B.index, 0u);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0u);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0u);
}

TEST_F(LineCode4b5bTest, Buffer_Of_Encoded_Bits_Should_Be_Ready_To_Read_A_Decoded_Byte) {
    lineCode4B5B.index = 0u;
    ASSERT_EQ(LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(&lineCode4B5B), 0u);

    lineCode4B5B.index = 1u;
    ASSERT_EQ(LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(&lineCode4B5B), 0u);

    lineCode4B5B.index = 10u;
    ASSERT_EQ(LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(&lineCode4B5B), 1u);

    lineCode4B5B.index = 11u;
    ASSERT_EQ(LRP_4B5B_isBufferOfEncodedBitsReadyToReadADecodedByte(&lineCode4B5B), 1u);
}

TEST_F(LineCode4b5bTest, Should_Be_Added_Encoded_Byte_To_Buffer_Of_Ecoded_Bits) {
    const unsigned char data = 0b10101010u;
    LRP_4B5B_reset(&lineCode4B5B);

    LRP_4B5B_addEncodedByteToBufferOfEncodedBits(&lineCode4B5B, &data);

    ASSERT_EQ(lineCode4B5B.buffer[0], data);
    ASSERT_EQ(lineCode4B5B.index, 8u);

    LRP_4B5B_addEncodedByteToBufferOfEncodedBits(&lineCode4B5B, &data);

    ASSERT_EQ(lineCode4B5B.buffer[1], data);
    ASSERT_EQ(lineCode4B5B.index, 16u);

    LRP_4B5B_reset(&lineCode4B5B);
    lineCode4B5B.buffer[0] = 0u;
    lineCode4B5B.buffer[1] = 0u;
    lineCode4B5B.index = 4u;

    LRP_4B5B_addEncodedByteToBufferOfEncodedBits(&lineCode4B5B, &data);

    ASSERT_EQ(lineCode4B5B.buffer[0], 0b10100000u);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0b00001010u);
    ASSERT_EQ(lineCode4B5B.index, 12u);
}

TEST_F(LineCode4b5bTest,
       Should_Be_Tried_To_Read_A_Decoded_Byte_From_Buffer_Of_Encoded_Bits_When_The_Decoding_Is_Failure) {
    lineCode4B5B.index = 16u;
    // 01101 is an invalid 4B5B code
    lineCode4B5B.buffer[0] = 0b10101101u;
    lineCode4B5B.buffer[1] = 0b10101001u;

    LRPLineCode4B5BDecodingState decodingState;
    const unsigned char decodedByte = LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(&lineCode4B5B,
                                                                                            &decodingState);

    ASSERT_EQ(decodedByte, 0u);
    ASSERT_TRUE(decodingState);

    ASSERT_EQ(lineCode4B5B.buffer[0], 0b00101010u);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0u);
    ASSERT_EQ(lineCode4B5B.index, 6u);
}

TEST_F(LineCode4b5bTest,
       Should_Be_Tried_To_Read_A_Decoded_Byte_From_Buffer_Of_Encoded_Bits_When_The_Decoding_Is_Successful) {
    lineCode4B5B.index = 16;
    lineCode4B5B.buffer[0] = 0b10111101u;
    lineCode4B5B.buffer[1] = 0b10101011u;

    LRPLineCode4B5BDecodingState decodingState;
    const unsigned char decodedByte = LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(&lineCode4B5B,
                                                                                            &decodingState);

    ASSERT_EQ(decodedByte, 0xFFu);
    ASSERT_FALSE(decodingState);

    ASSERT_EQ(lineCode4B5B.buffer[0], 0b00101010u);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0u);
    ASSERT_EQ(lineCode4B5B.index, 6u);
}

TEST_F(LineCode4b5bTest, Should_Be_Tested_Status_Of_Decoding_Failed) {
    // FAILED_TO_DECODED macro in line_code_4b5b.c
    const LRPLineCode4B5BDecodingState failedDecodingState = LRPLineCode4B5BDecodingState_FailedDecoding;
    // SUCCEED_TO_DECODED macro in line_code_4b5b.c
    const LRPLineCode4B5BDecodingState succeededDecodingState = LRPLineCode4B5BDecodingState_SuccessfulDecoding;

    ASSERT_TRUE(LRP_4B5B_isDecodingFailed(&failedDecodingState));

    ASSERT_FALSE(LRP_4B5B_isDecodingFailed(&succeededDecodingState));
}

TEST_F(LineCode4b5bTest, Buffer_Of_Encoded_Bits_Should_Be_Ready_To_Add_The_Next_Byte_To_Encode) {
    lineCode4B5B.index = 0u;
    ASSERT_TRUE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));

    lineCode4B5B.index = 5u;
    ASSERT_TRUE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));

    lineCode4B5B.index = 6u;
    ASSERT_TRUE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));

    lineCode4B5B.index = 7u;
    ASSERT_FALSE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));

    lineCode4B5B.index = 8u;
    ASSERT_FALSE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));
}

TEST_F(LineCode4b5bTest, Should_Be_Encoded_Data_Byte_And_Add_It_To_Buffer_Of_Encoded_Bits) {
    const unsigned char dataToBeEncoded = 0xFFu;
    LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(&lineCode4B5B, &dataToBeEncoded);

    ASSERT_EQ(lineCode4B5B.index, 10u);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0b10111101u);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0b00000011u);

    lineCode4B5B.index = 0;
    lineCode4B5B.buffer[0] =
    lineCode4B5B.buffer[1] = 0;

    const unsigned char dataToBeNotEncoded = 0x00u;
    LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(&lineCode4B5B, &dataToBeNotEncoded);

    ASSERT_EQ(lineCode4B5B.index, 10u);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0b11011110u);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0b00000011u);
}

TEST_F(LineCode4b5bTest, Should_Be_Read_An_Encoded_Byte_From_Buffer_Of_Encoded_Bits) {
    lineCode4B5B.index = 16u;
    lineCode4B5B.buffer[0] = 0xFFu;
    lineCode4B5B.buffer[1] = 0xFEu;

    const unsigned char encodedByte = LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(&lineCode4B5B);

    ASSERT_EQ(encodedByte, 0xFFu);
    ASSERT_EQ(lineCode4B5B.index, 8u);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0xFEu);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0u);
}

TEST_F(LineCode4b5bTest, Should_Be_Checked_Remaining_Bits_In_Buffer_Of_Encoded_Bits) {
    ASSERT_FALSE(LRP_4B5B_haveRemainingBitsInBufferOfEncodedBits(&lineCode4B5B));

    lineCode4B5B.index = 1u;
    ASSERT_TRUE(LRP_4B5B_haveRemainingBitsInBufferOfEncodedBits(&lineCode4B5B));
}

TEST_F(LineCode4b5bTest, Should_Be_Read_Remaining_Bits_From_Buffer_Of_Encoded_Bits) {
    lineCode4B5B.index = 1u;
    lineCode4B5B.buffer[0] = 0xFFu;
    lineCode4B5B.buffer[1] = 0xFEu;

    const unsigned char remainingBits = LRP_4B5B_readARemainingBitsFromBufferOfEncodedBits(&lineCode4B5B);

    ASSERT_EQ(remainingBits, 0xFFu);
    ASSERT_EQ(lineCode4B5B.index, 0u);
}