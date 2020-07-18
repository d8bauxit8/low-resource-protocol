#include <gtest/gtest.h>
#include "../src/link_layer.h"

class LinkLayerTest : public ::testing::Test {
protected:
    _LRPSessionProvider sessionProvider{};
};

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Skip) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
}

TEST_F(LinkLayerTest, Should_Be_Set_Status_To_Ok) {
    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);
}

TEST_F(LinkLayerTest, Should_Be_Checked_The_Statuse_Is_Ok) {
    LRP_LinkLayer_setSkip(&sessionProvider);

    ASSERT_EQ(LRP_LinkLayer_isStatusOK(&sessionProvider), 0);

    LRP_LinkLayer_setOk(&sessionProvider);

    ASSERT_EQ(LRP_LinkLayer_isStatusOK(&sessionProvider), 1);
}