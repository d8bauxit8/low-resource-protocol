#include <gtest/gtest.h>
#include "../src/session-provider.h"

class SessionProviderTest : public ::testing::Test {
protected:
    LRPSessionProvider sessionProvider{};
    unsigned char sourceDeviceId = 0b10100u;
    LRPFrame frameBuffer[3]{};
};

TEST_F(SessionProviderTest, should_be_initialized) {
    LRP_SessionProvider_init(&sessionProvider, &sourceDeviceId, frameBuffer, 3);

    ASSERT_EQ(sessionProvider.deviceId, &sourceDeviceId);
    ASSERT_EQ(*sessionProvider.deviceId, sourceDeviceId);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LRP_LINK_LAYER_NO_ERROR);

    ASSERT_EQ(sessionProvider.frameBuffer, frameBuffer);

    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(sessionProvider.linkCurrentFrame, &frameBuffer[0]);
    ASSERT_EQ(sessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    ASSERT_EQ(sessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}