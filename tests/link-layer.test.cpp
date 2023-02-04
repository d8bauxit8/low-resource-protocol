#include <gtest/gtest.h>
#include "../src/link-layer.h"

class LinkLayerTest : public ::testing::Test {
protected:
    LRPSessionProvider sessionProvider{};
};

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Skip) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
}

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Ok) {
    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_OK);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
}

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Error) {
    LRP_LinkLayer_setError(&sessionProvider, LRP_LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_ERROR);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NOISE_STROKE_ERROR);

    LRP_LinkLayer_setError(&sessionProvider, LRP_LINK_LAYER_DECODE_ERROR);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_ERROR);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_DECODE_ERROR);
}

TEST_F(LinkLayerTest, Should_Be_Checked_The_Statuse_Is_Ok) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusOK(&sessionProvider));

    LRP_LinkLayer_setError(&sessionProvider, LRP_LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_FALSE(LRP_LinkLayer_isStatusOK(&sessionProvider));

    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_TRUE(LRP_LinkLayer_isStatusOK(&sessionProvider));
}

TEST_F(LinkLayerTest, Should_Be_Checked_The_Statuse_Is_Error) {
    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, LRP_LINK_LAYER_NO_ERROR));

    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, LRP_LINK_LAYER_NO_ERROR));

    LRP_LinkLayer_setError(&sessionProvider, LRP_LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_TRUE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, LRP_LINK_LAYER_NOISE_STROKE_ERROR));

    LRP_LinkLayer_setError(&sessionProvider, LRP_LINK_LAYER_DECODE_ERROR);

    ASSERT_TRUE(LRP_LinkLayer_isStatusError(&sessionProvider));
    ASSERT_TRUE(LRP_LinkLayer_isError(&sessionProvider, LRP_LINK_LAYER_DECODE_ERROR));
}