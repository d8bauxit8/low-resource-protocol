#include <gtest/gtest.h>
#include "../src/link-layer.h"

class LinkLayerTest : public ::testing::Test {
protected:
    LRPSessionProvider sessionProvider{};
};

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Skip) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRPLinkLayerStatus_Skip);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRPLinkLayerErrorCode_NoError);
}

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Ok) {
    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRPLinkLayerStatus_OK);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRPLinkLayerErrorCode_NoError);
}

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Error) {
    LRP_LinkLayer_setError(&sessionProvider, LRPLinkLayerErrorCode_NoiseStrokeError);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRPLinkLayerStatus_Error);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRPLinkLayerErrorCode_NoiseStrokeError);

    LRP_LinkLayer_setError(&sessionProvider, LRPLinkLayerErrorCode_DecodeError);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRPLinkLayerStatus_Error);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRPLinkLayerErrorCode_DecodeError);
}

TEST_F(LinkLayerTest, Should_Be_Checked_The_Statuse_Is_Ok) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusOK(&sessionProvider));

    LRP_LinkLayer_setError(&sessionProvider, LRPLinkLayerErrorCode_NoiseStrokeError);

    ASSERT_FALSE(LRP_LinkLayer_isStatusOK(&sessionProvider));

    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_TRUE(LRP_LinkLayer_isStatusOK(&sessionProvider));
}

TEST_F(LinkLayerTest, Should_Be_Checked_The_Statuse_Is_Error) {
    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, LRPLinkLayerErrorCode_NoError));

    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, LRPLinkLayerErrorCode_NoError));

    LRP_LinkLayer_setError(&sessionProvider, LRPLinkLayerErrorCode_NoiseStrokeError);

    ASSERT_TRUE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, LRPLinkLayerErrorCode_NoiseStrokeError));

    LRP_LinkLayer_setError(&sessionProvider, LRPLinkLayerErrorCode_DecodeError);

    ASSERT_TRUE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, LRPLinkLayerErrorCode_DecodeError));
}