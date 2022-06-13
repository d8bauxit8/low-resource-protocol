#include <gtest/gtest.h>
#include "../src/session-provider.h"

class SessionProviderTest : public ::testing::Test {
protected:
    LRPSessionProvider sessionProvider{};
    const unsigned char sourceDeviceId = 0b10100u;
    LRPFrame frameBuffer[3]{};
};

TEST_F(SessionProviderTest, Should_Be_Initialized) {
    LRP_SessionProvider_init(&sessionProvider, &sourceDeviceId, frameBuffer, 3);

    ASSERT_EQ(sessionProvider.deviceId, &sourceDeviceId);
    ASSERT_EQ(*sessionProvider.deviceId, sourceDeviceId);

    ASSERT_EQ(sessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(sessionProvider.linkLayerErrorCode, LINK_LAYER_NO_ERROR);

    ASSERT_EQ(sessionProvider.frameBuffer, frameBuffer);

    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

    ASSERT_EQ(sessionProvider.linkCurrentFrame, &frameBuffer[0]);
    ASSERT_EQ(sessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    ASSERT_EQ(sessionProvider.applicationCurrentFrame, &frameBuffer[0]);
}