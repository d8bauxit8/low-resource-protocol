#include <gtest/gtest.h>
#include "../src/line_code_4b5b.h"

class LineCode4b5bTest : public ::testing::Test {
protected:

    LRPLineCode4B5B lineCode4B5B{};
    unsigned char buffer[2]{};

    void SetUp() override {
        lineCode4B5B.index = 0;
        lineCode4B5B.buffer[0] = 0;
        lineCode4B5B.buffer[1] = 0;
    }
};

TEST_F(LineCode4b5bTest, Should_Be_Reseted) {
    lineCode4B5B.index = 2;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    LRP_4B5B_reset(&lineCode4B5B);

    ASSERT_EQ(lineCode4B5B.index, 0);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0);
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
    const unsigned char data = 0b10101010;
    LRP_4B5B_reset(&lineCode4B5B);

    LRP_4B5B_addEncodedByteToBufferOfEncodedBits(&lineCode4B5B, &data);

    ASSERT_EQ(lineCode4B5B.buffer[0], data);
    ASSERT_EQ(lineCode4B5B.index, 8);

    LRP_4B5B_addEncodedByteToBufferOfEncodedBits(&lineCode4B5B, &data);

    ASSERT_EQ(lineCode4B5B.buffer[1], data);
    ASSERT_EQ(lineCode4B5B.index, 16);

    LRP_4B5B_reset(&lineCode4B5B);
    lineCode4B5B.buffer[0] = 0;
    lineCode4B5B.buffer[1] = 0;
    lineCode4B5B.index = 4;

    LRP_4B5B_addEncodedByteToBufferOfEncodedBits(&lineCode4B5B, &data);

    ASSERT_EQ(lineCode4B5B.buffer[0], 0b10100000);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0b00001010);
    ASSERT_EQ(lineCode4B5B.index, 12);
}

TEST_F(LineCode4b5bTest,
       Should_Be_Tried_To_Read_A_Decoded_Byte_From_Buffer_Of_Encoded_Bits_When_The_Decoding_Is_Failure) {
    lineCode4B5B.index = 16;
    // 01101 is an invalid 4B5B code
    lineCode4B5B.buffer[0] = 0b10101101;
    lineCode4B5B.buffer[1] = 0b10101001;

    unsigned char decodingState;
    const unsigned char decodedByte = LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(&lineCode4B5B,
                                                                                            &decodingState);

    ASSERT_EQ(decodedByte, 0u);
    ASSERT_TRUE(decodingState);

    ASSERT_EQ(lineCode4B5B.buffer[0], 0b00101010);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0);
    ASSERT_EQ(lineCode4B5B.index, 6);
}

TEST_F(LineCode4b5bTest,
       Should_Be_Tried_To_Read_A_Decoded_Byte_From_Buffer_Of_Encoded_Bits_When_The_Decoding_Is_Successful) {
    lineCode4B5B.index = 16;
    lineCode4B5B.buffer[0] = 0b10111101;
    lineCode4B5B.buffer[1] = 0b10101011;

    unsigned char decodingState;
    const unsigned char decodedByte = LRP_4B5B_tryToReadADecodedByteFromBufferOfEncodedBits(&lineCode4B5B,
                                                                                            &decodingState);

    ASSERT_EQ(decodedByte, 0xFF);
    ASSERT_FALSE(decodingState);

    ASSERT_EQ(lineCode4B5B.buffer[0], 0b00101010);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0);
    ASSERT_EQ(lineCode4B5B.index, 6);
}

TEST_F(LineCode4b5bTest, Should_Be_Tested_Status_Of_Decoding_Failed) {
    // FAILED_TO_DECODED macro in line_code_4b5b.c
    const unsigned char failedDecodingState = 1;
    // SUCCEED_TO_DECODED macro in line_code_4b5b.c
    const unsigned char succeededDecodingState = 0;

    ASSERT_TRUE(LRP_4B5B_isDecodingFailed(&failedDecodingState));

    ASSERT_FALSE(LRP_4B5B_isDecodingFailed(&succeededDecodingState));
}

TEST_F(LineCode4b5bTest, Buffer_Of_Encoded_Bits_Should_Be_Ready_To_Add_The_Next_Byte_To_Encode) {
    lineCode4B5B.index = 0;
    ASSERT_TRUE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));

    lineCode4B5B.index = 5;
    ASSERT_TRUE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));

    lineCode4B5B.index = 6;
    ASSERT_TRUE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));

    lineCode4B5B.index = 7;
    ASSERT_FALSE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));

    lineCode4B5B.index = 8;
    ASSERT_FALSE(LRP_4B5B_isBufferOfEncodedBitsReadyToAddTheNextByteToEncode(&lineCode4B5B));
}

TEST_F(LineCode4b5bTest, Should_Be_Encoded_Data_Byte_And_Add_It_To_Buffer_Of_Encoded_Bits) {
    const unsigned char dataToBeEncoded = 0xFF;
    LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(&lineCode4B5B, &dataToBeEncoded);

    ASSERT_EQ(lineCode4B5B.index, 10);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0b10111101);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0b00000011);

    lineCode4B5B.index = 0;
    lineCode4B5B.buffer[0] =
    lineCode4B5B.buffer[1] = 0;

    const unsigned char dataToBeNotEncoded = 0x00;
    LRP_4B5B_encodeDataByteAndAddItToBufferOfEncodedBits(&lineCode4B5B, &dataToBeNotEncoded);

    ASSERT_EQ(lineCode4B5B.index, 10);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0b11011110);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0b00000011);
}

TEST_F(LineCode4b5bTest, Should_Be_Read_An_Encoded_Byte_From_Buffer_Of_Encoded_Bits) {
    lineCode4B5B.index = 16;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    const unsigned char encodedByte = LRP_4B5B_readAnEncodedByteFromBufferOfEncodedBits(&lineCode4B5B);

    ASSERT_EQ(encodedByte, 0xFF);
    ASSERT_EQ(lineCode4B5B.index, 8);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0xFE);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0);
}

TEST_F(LineCode4b5bTest, Should_Be_Checked_Remaining_Bits_In_Buffer_Of_Encoded_Bits) {
    ASSERT_FALSE(LRP_4B5B_isThereRemainingBitsInBufferOfEncodedBits(&lineCode4B5B));

    lineCode4B5B.index = 1;
    ASSERT_TRUE(LRP_4B5B_isThereRemainingBitsInBufferOfEncodedBits(&lineCode4B5B));
}

TEST_F(LineCode4b5bTest, Should_Be_Read_Remaining_Bits_From_Buffer_Of_Encoded_Bits) {
    lineCode4B5B.index = 1;
    lineCode4B5B.buffer[0] = 0xFF;
    lineCode4B5B.buffer[1] = 0xFE;

    const unsigned char remainingBits = LRP_4B5B_readAnRemainingBitsFromBufferOfEncodedBits(&lineCode4B5B);

    ASSERT_EQ(remainingBits, 0xFF);
    ASSERT_EQ(lineCode4B5B.index, 0);
}