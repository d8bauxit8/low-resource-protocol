#include <gtest/gtest.h>
#include "../src/parity-bit.h"

class ParityBitTest : public ::testing::Test {
protected:
    unsigned char data = 0u;
    // Unknown value
    unsigned char oddParityBit = 0xFFu;
};

TEST_F(ParityBitTest, Should_Be_Created_Odd_Parity_Bit) {
    data = 0b11001101u;
    oddParityBit = LRP_ParityBit_createOddParityBit(data);
    ASSERT_EQ(oddParityBit, 0u);

    data = 0b11001100u;
    oddParityBit = LRP_ParityBit_createOddParityBit(data);
    ASSERT_EQ(oddParityBit, 1u);
}

TEST_F(ParityBitTest, Should_Be_Check_The_Odd_Parity_Bit_Is_Invalid) {
    // Data whose parity bit should be odd
    data = 0b11001101u;

    // Odd parity bit flag
    oddParityBit = 0u;

    ASSERT_FALSE(LRP_ParityBit_isInvalidOddParityBit(data, &oddParityBit));

    // Even parity bit flag
    oddParityBit = 1u;
    ASSERT_TRUE(LRP_ParityBit_isInvalidOddParityBit(data, &oddParityBit));
}