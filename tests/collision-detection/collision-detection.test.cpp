#include <gtest/gtest.h>
#include "../../src/collision-detection/collision-detection.h"

class CollisionDetectionTest : public ::testing::Test {
protected:

    const unsigned char sourceDeviceId = 0b10100;

    LRPReceiveSessionProvider receiveSessionProvider{};
    LRPFrame receiveFrameBuffer[3]{};

    LRPTransmitSessionProvider transmitSessionProvider{};
    LRPFrame transmitFrameBuffer[3]{};

    LRPCollisionDetection collisionDetection;

    void SetUp() override {
        LRP_SessionProvider_init((LRPSessionProvider *) &receiveSessionProvider, &sourceDeviceId, receiveFrameBuffer,
                                 3);
        LRP_SessionProvider_init((LRPSessionProvider *) &transmitSessionProvider, &sourceDeviceId, transmitFrameBuffer,
                                 3);
        LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);
    }
};

TEST_F(CollisionDetectionTest, Should_Be_Initialized) {
    ASSERT_EQ(collisionDetection.backoffTime, 0);
    ASSERT_EQ(collisionDetection.numberOfCollisions, 0);
    ASSERT_EQ(collisionDetection.transmitSessionProvider, &transmitSessionProvider);
    ASSERT_EQ(collisionDetection.receiveSessionProvider, &receiveSessionProvider);
}

TEST_F(CollisionDetectionTest, Should_Restart_Transmit_Module) {
    collisionDetection.backoffTime = 2u;

    ASSERT_EQ(LRP_CollisionDetection_isRestartTransmitModule(&collisionDetection), 0);
    ASSERT_EQ(collisionDetection.backoffTime, 1u);
    ASSERT_EQ(LRP_CollisionDetection_isRestartTransmitModule(&collisionDetection), 0);
    ASSERT_EQ(collisionDetection.backoffTime, 0u);
    ASSERT_EQ(LRP_CollisionDetection_isRestartTransmitModule(&collisionDetection), 1);
    ASSERT_EQ(collisionDetection.backoffTime, 0u);
}

TEST_F(CollisionDetectionTest, When_Receive_And_Transmit_Layer_Have_Not_Error_Should_IsDecodeError_Return_True) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                           LINK_LAYER_DECODE_ERROR);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                           LINK_LAYER_DECODE_ERROR);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiveSessionProvider),
              1);
    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitSessionProvider),
              1);
}

TEST_F(CollisionDetectionTest, When_Receive_Layer_Has_Decode_Error_Should_Handle_Decode_Error) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                           LINK_LAYER_DECODE_ERROR);
    LRP_LinkLayer_setSkip((LRPSessionProvider *) collisionDetection.transmitSessionProvider);

    LRP_CollisionDetection_decodeErrorHandler(&collisionDetection);

    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_DECODE_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_DECODE_ERROR), 1);
}

TEST_F(CollisionDetectionTest, When_Receive_And_Transmit_Layer_Have_Not_Error_Should_Not_Handle_Decode_Error) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                           LINK_LAYER_NO_ERROR);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                           LINK_LAYER_NO_ERROR);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiveSessionProvider),
              0);
    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitSessionProvider),
              0);
}

TEST_F(CollisionDetectionTest, When_Receive_And_Transmit_Layer_Have_Noise_Stroke_Error_Should_Not_Handle_Decode_Error) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                           LINK_LAYER_NOISE_STROKE_ERROR);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                           LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiveSessionProvider),
              0);
    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitSessionProvider),
              0);
}

TEST_F(CollisionDetectionTest, When_Data_Is_A_Noise_Stroke_Should_IsNoiseStrokeError_Return_True) {
    unsigned char data = COLLISION_DETECTION_NOISE_STROKE;

    ASSERT_EQ(LRP_CollisionDetection_isNoiseStrokeError(&data), 1);
}

TEST_F(CollisionDetectionTest, When_Data_Is_A_Noise_Stroke_Should_IsNoiseStrokeError_Return_False) {
    unsigned char data = 0;

    ASSERT_EQ(LRP_CollisionDetection_isNoiseStrokeError(&data), 0);
}

TEST_F(CollisionDetectionTest, When_NoiseStrokeErrorHandler_Is_Called_Set_Noise_Stroke_Error_And_Set_Backoff_Time) {
    LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);

    ASSERT_EQ(collisionDetection.numberOfCollisions, 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_NOISE_STROKE_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_NOISE_STROKE_ERROR), 1);
}
