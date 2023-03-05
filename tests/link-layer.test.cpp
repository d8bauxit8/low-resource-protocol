#include <gtest/gtest.h>
#include "../src/link-layer.h"

class LinkLayerTest : public ::testing::Test {
protected:
    LRPSessionProvider sessionProvider{};
};

TEST_F(LinkLayerTest, when_setSkip_is_called_linkLayerStatus_should_be_skip) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
}

TEST_F(LinkLayerTest, when_setOk_is_called_linkLayerStatus_should_be_OK) {
    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_OK);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);
}

TEST_F(LinkLayerTest, when_setError_is_called_linkLayerStatus_should_be_error) {
    LRP_LinkLayer_setError(&sessionProvider, LRP_LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_ERROR);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NOISE_STROKE_ERROR);

    LRP_LinkLayer_setError(&sessionProvider, LRP_LINK_LAYER_DECODE_ERROR);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_ERROR);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_DECODE_ERROR);
}

TEST_F(LinkLayerTest, should_be_checked_the_status_right) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_FALSE(LRP_LinkLayer_isStatusOK(&sessionProvider));

    LRP_LinkLayer_setError(&sessionProvider, LRP_LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_FALSE(LRP_LinkLayer_isStatusOK(&sessionProvider));

    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_TRUE(LRP_LinkLayer_isStatusOK(&sessionProvider));
}

TEST_F(LinkLayerTest, should_be_checked_the_status_errors) {
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