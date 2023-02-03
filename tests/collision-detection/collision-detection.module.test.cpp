#include <gtest/gtest.h>
#include "../../src/collision-detection/collision-detection.module.h"
#include "../../src/receiver/receiver.module.h"
#include "../../src/transmitter/transmitter.module.h"

class CollisionDetectionTest : public ::testing::Test {
protected:

    unsigned char sourceDeviceId = 0b10100;
    unsigned char groupId = 0b11100;
    const unsigned char maxNumberOfCollisions = 8;

    LRPReceiverSessionProvider receiverSessionProvider{};
    LRPFrame receiverFrameBuffer[3]{};

    LRPTransmitterSessionProvider transmitterSessionProvider{};
    LRPFrame transmitterFrameBuffer[3]{};

    LRPCollisionDetection collisionDetection{};

    void SetUp() override {
        LRP_Receiver_init(&receiverSessionProvider, &sourceDeviceId, receiverFrameBuffer,
                          3, &groupId);
        LRP_Transmitter_init(&transmitterSessionProvider, &sourceDeviceId,
                             transmitterFrameBuffer,
                             3);
        LRP_CollisionDetection_init(&collisionDetection, &transmitterSessionProvider, &receiverSessionProvider);
    }
};

TEST_F(CollisionDetectionTest, Should_Be_Initialized) {
    ASSERT_EQ(collisionDetection.backoffTime, 0);
    ASSERT_EQ(collisionDetection.numberOfCollisions, 0);
    ASSERT_EQ(collisionDetection.transmitterSessionProvider, &transmitterSessionProvider);
    ASSERT_EQ(collisionDetection.receiverSessionProvider, &receiverSessionProvider);
}

TEST_F(CollisionDetectionTest, Should_Restart_Transmitter_Module) {
    collisionDetection.backoffTime = 2u;

    ASSERT_EQ(LRP_CollisionDetection_shouldRestartTransmitterModule(&collisionDetection), 0);
    ASSERT_EQ(collisionDetection.backoffTime, 1u);
    ASSERT_EQ(LRP_CollisionDetection_shouldRestartTransmitterModule(&collisionDetection), 0);
    ASSERT_EQ(collisionDetection.backoffTime, 0u);
    ASSERT_EQ(LRP_CollisionDetection_shouldRestartTransmitterModule(&collisionDetection), 1);
    ASSERT_EQ(collisionDetection.backoffTime, 0u);
}

TEST_F(CollisionDetectionTest, When_Receiver_And_Transmitter_Layer_Have_Not_Error_Should_IsDecodeError_Return_True) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                           DecodeError);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                           DecodeError);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiverSessionProvider),
              1);
    ASSERT_EQ(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider),
            1);
}

TEST_F(CollisionDetectionTest, When_Receiver_Layer_Has_Decode_Error_Should_Handle_Decode_Error) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                           DecodeError);
    LRP_LinkLayer_setSkip((LRPSessionProvider *) collisionDetection.transmitterSessionProvider);

    LRP_CollisionDetection_decodeErrorHandler(&collisionDetection);

    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                                    DecodeError), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                                    DecodeError), 1);
}

TEST_F(CollisionDetectionTest, When_Receiver_And_Transmitter_Layer_Have_Not_Error_Should_Not_Handle_Decode_Error) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                           NoError);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                           NoError);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiverSessionProvider),
              0);
    ASSERT_EQ(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider),
            0);
}

TEST_F(CollisionDetectionTest,
       When_Receiver_And_Transmitter_Layer_Have_Noise_Stroke_Error_Should_Not_Handle_Decode_Error) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                           NoiseStrokeError);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                           NoiseStrokeError);

    ASSERT_EQ(LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiverSessionProvider),
              0);
    ASSERT_EQ(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider),
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
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                                    NoiseStrokeError), 1);
    ASSERT_EQ(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                                    NoiseStrokeError), 1);
}

TEST_F(CollisionDetectionTest,
       When_NumberOfCollisions_Is_Reached_The_Maximum_Value_Should_LRP_CollisionDetection_isReachedMaxNumberOfCollisions_Method_Return_True) {
    for (int i = 0; i < maxNumberOfCollisions; ++i) {
        LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);
    }

    ASSERT_EQ(collisionDetection.numberOfCollisions, maxNumberOfCollisions);
    ASSERT_EQ(LRP_CollisionDetection_isReachedMaxNumberOfCollisions(&collisionDetection), 1);
}

TEST_F(CollisionDetectionTest,
       When_NumberOfCollisions_Is_Bigger_Than_The_Maximum_Value_Should_LRP_CollisionDetection_isReachedMaxNumberOfCollisions_Method_Return_True) {
    unsigned char maxNumberOfCollisionsPlusOne = maxNumberOfCollisions + 1;
    for (int i = 0; i < maxNumberOfCollisionsPlusOne; ++i) {
        LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);
    }

    ASSERT_EQ(collisionDetection.numberOfCollisions, maxNumberOfCollisions);
    ASSERT_EQ(LRP_CollisionDetection_isReachedMaxNumberOfCollisions(&collisionDetection), 1);
}

TEST_F(CollisionDetectionTest,
       When_NumberOfCollisions_Is_Not_Reached_The_Maximum_Value_Should_LRP_CollisionDetection_isReachedMaxNumberOfCollisions_Method_Return_False) {
    unsigned char maxNumberOfCollisionsMinusOne = maxNumberOfCollisions - 1;
    for (int i = 0; i < maxNumberOfCollisionsMinusOne; ++i) {
        LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);
    }

    ASSERT_EQ(collisionDetection.numberOfCollisions, maxNumberOfCollisionsMinusOne);
    ASSERT_EQ(LRP_CollisionDetection_isReachedMaxNumberOfCollisions(&collisionDetection), 0);
}