#include <gtest/gtest.h>
#include "../src/parity_bit.h"

class ParityBitTest : public ::testing::Test {
protected:
    unsigned char data;
    // Unknown value
    unsigned char oddParityBit = 0xFF;
};

TEST_F(ParityBitTest, Should_Be_Created_Odd_Parity_Bit) {
    data = 0b11001101;
    oddParityBit = LRP_ParityBit_createOddParityBit(data);
    ASSERT_EQ(oddParityBit, 1);

    data = 0b11001100;
    oddParityBit = LRP_ParityBit_createOddParityBit(data);
    ASSERT_EQ(oddParityBit, 0);
}

TEST_F(ParityBitTest, Should_Be_Check_The_Odd_Parity_Bit_Is_Invalid) {
    // Data whose parity bit should be odd
    data = 0b11001101;

    // Odd parity bit flag
    oddParityBit = 0;

    ASSERT_EQ(LRP_ParityBit_isInvalidOddParityBit(data, &oddParityBit), 0);

    // Even parity bit flag
    oddParityBit = 1;
    ASSERT_EQ(LRP_ParityBit_isInvalidOddParityBit(data, &oddParityBit), 1);
}