#include <gtest/gtest.h>
#include "../src/session_provider.h"

class SessionProviderTest : public ::testing::Test {
protected:
    _LRPSessionProvider sessionProvider{};
    unsigned char deviceId = 0;
    _LRPFrame frameBuffer[3]{};
};

TEST_F(SessionProviderTest, Should_Be_Initialized) {
    LRP_SessionProvider_init(&sessionProvider, &deviceId, frameBuffer, 3);

    ASSERT_EQ(sessionProvider.deviceId, &deviceId);
    ASSERT_EQ(*sessionProvider.deviceId, deviceId);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);

    ASSERT_EQ(sessionProvider.frameBuffer, frameBuffer);

    ASSERT_EQ(sessionProvider.linkCurrentFrame, &frameBuffer[0]);
    ASSERT_EQ(sessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    ASSERT_EQ(sessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}