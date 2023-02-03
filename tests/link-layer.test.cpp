#include <gtest/gtest.h>
#include "../src/link-layer.h"

class LinkLayerTest : public ::testing::Test {
protected:
    LRPSessionProvider sessionProvider{};
};

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Skip) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, NoError);
}

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Ok) {
    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, OK);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, NoError);
}

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Error) {
    LRP_LinkLayer_setError(&sessionProvider, NoiseStrokeError);

    ASSERT_EQ(sessionProvider.linkLayerStatus, Error);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, NoiseStrokeError);

    LRP_LinkLayer_setError(&sessionProvider, DecodeError);

    ASSERT_EQ(sessionProvider.linkLayerStatus, Error);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, DecodeError);
}

TEST_F(LinkLayerTest, Should_Be_Checked_The_Statuse_Is_Ok) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusOK(&sessionProvider));

    LRP_LinkLayer_setError(&sessionProvider, NoiseStrokeError);

    ASSERT_FALSE(LRP_LinkLayer_isStatusOK(&sessionProvider));

    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_TRUE(LRP_LinkLayer_isStatusOK(&sessionProvider));
}

TEST_F(LinkLayerTest, Should_Be_Checked_The_Statuse_Is_Error) {
    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, NoError));

    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, NoError));

    LRP_LinkLayer_setError(&sessionProvider, NoiseStrokeError);

    ASSERT_TRUE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, NoiseStrokeError));

    LRP_LinkLayer_setError(&sessionProvider, DecodeError);

    ASSERT_TRUE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, DecodeError));
}