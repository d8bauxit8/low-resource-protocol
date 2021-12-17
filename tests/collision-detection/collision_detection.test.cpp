#include <gtest/gtest.h>
#include "../../src/collision-detection/collision_detection.h"

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
    }
};

TEST_F(CollisionDetectionTest, Should_Be_Initialized) {
    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    ASSERT_EQ(collisionDetection.delay, 0);
    ASSERT_EQ(collisionDetection.transmitSessionProvider, &transmitSessionProvider);
    ASSERT_EQ(collisionDetection.receiveSessionProvider, &receiveSessionProvider);
}

TEST_F(CollisionDetectionTest, Should_Restart_Transmit_Module) {
    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    collisionDetection.delay = 0b00000010;

    ASSERT_EQ(LRP_CollisionDetection_isRestartTransmitModule(&collisionDetection), 1);
    ASSERT_EQ(collisionDetection.delay, 0b00000010);
}

TEST_F(CollisionDetectionTest, Should_Not_Restart_Transmit_Module) {
    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    collisionDetection.delay = 0b00000011;

    ASSERT_EQ(LRP_CollisionDetection_isRestartTransmitModule(&collisionDetection), 0);
    ASSERT_EQ(collisionDetection.delay, 0b00000001);
}

TEST_F(CollisionDetectionTest, Should_Decode_Error_Handler_When_Receive_Layer_Has_Decode_Error) {
    unsigned char data = 0;

    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                           LINK_LAYER_DECODE_ERROR);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeErrorHandler(&collisionDetection, &data), 1);
    // This code (COLLISION_DETECTION_NOISE_STROKE) available in the collision-detection.c file
    ASSERT_EQ(data, 0b00100000u);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_NOISE_STROKE_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_NOISE_STROKE_ERROR), 1);
}

TEST_F(CollisionDetectionTest, Should_Not_Decode_Error_Handler_When_Receive_Layer_Has_Not_Error) {
    unsigned char data = 0;

    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                           LINK_LAYER_NO_ERROR);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeErrorHandler(&collisionDetection, &data), 0);
    ASSERT_EQ(data, 0);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_NO_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_NO_ERROR), 1);
}

TEST_F(CollisionDetectionTest, Should_Not_Decode_Error_Handler_When_Receive_Layer_Has_Noise_Stroke_Error) {
    unsigned char data = 0;

    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                           LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeErrorHandler(&collisionDetection, &data), 0);
    ASSERT_EQ(data, 0);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_NOISE_STROKE_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_NO_ERROR), 1);
}

TEST_F(CollisionDetectionTest, Should_Not_Decode_Error_Handler_When_Transmit_Layer_Has_Not_Error) {
    unsigned char data = 0;

    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                           LINK_LAYER_NO_ERROR);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeErrorHandler(&collisionDetection, &data), 0);
    ASSERT_EQ(data, 0);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_NO_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_NO_ERROR), 1);
}

TEST_F(CollisionDetectionTest, Should_Not_Decode_Error_Handler_When_Transmit_Layer_Has_Noise_Stroke_Error) {
    unsigned char data = 0;

    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                           LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeErrorHandler(&collisionDetection, &data), 0);
    ASSERT_EQ(data, 0);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_NO_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_NOISE_STROKE_ERROR), 1);
}

TEST_F(CollisionDetectionTest, Should_Handle_Noise_Stroke) {
    // This code (COLLISION_DETECTION_NOISE_STROKE) available in the collision-detection.c file
    unsigned char data = 0b00100000u;

    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    ASSERT_EQ(LRP_CollisionDetection_isNoiseStrokeErrorHandler(&collisionDetection, &data), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_NOISE_STROKE_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_NOISE_STROKE_ERROR), 1);
}

TEST_F(CollisionDetectionTest, Should_Not_Handle_Noise_Stroke) {
    unsigned char data = 0;

    LRP_CollisionDetection_init(&collisionDetection, &transmitSessionProvider, &receiveSessionProvider);

    ASSERT_EQ(LRP_CollisionDetection_isNoiseStrokeErrorHandler(&collisionDetection, &data), 0);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiveSessionProvider,
                                    LINK_LAYER_NO_ERROR), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitSessionProvider,
                                    LINK_LAYER_NO_ERROR), 1);
}
