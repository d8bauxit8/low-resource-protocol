#include <gtest/gtest.h>
#include "../../src/utilities/parity-bit.utility.h"

class ParityBitTest : public ::testing::Test {
protected:
    unsigned char data = 0u;
    // Unknown value
    unsigned char oddParityBit = 0xFFu;
};

TEST_F(ParityBitTest, odd_parity_bit_should_be_created) {
    data = 0b11001101u;
    oddParityBit = LRP_ParityBit_createOddParityBit(data);
    ASSERT_EQ(oddParityBit, 0u);

    data = 0b11001100u;
    oddParityBit = LRP_ParityBit_createOddParityBit(data);
    ASSERT_EQ(oddParityBit, 1u);
}

TEST_F(ParityBitTest, odd_parity_bit_should_be_checked) {
    // Data whose parity bit should be odd
    data = 0b11001101u;

    // Odd parity bit flag
    oddParityBit = 0u;

    ASSERT_FALSE(LRP_ParityBit_isInvalidOddParityBit(data, &oddParityBit));

    // Even parity bit flag
    oddParityBit = 1u;
    ASSERT_TRUE(LRP_ParityBit_isInvalidOddParityBit(data, &oddParityBit));
}