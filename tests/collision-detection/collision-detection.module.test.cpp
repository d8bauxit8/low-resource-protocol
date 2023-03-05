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

TEST_F(CollisionDetectionTest, should_be_initialized) {
    ASSERT_EQ(collisionDetection.backoffTime, 0);
    ASSERT_EQ(collisionDetection.numberOfCollisions, 0);
    ASSERT_EQ(collisionDetection.transmitterSessionProvider, &transmitterSessionProvider);
    ASSERT_EQ(collisionDetection.receiverSessionProvider, &receiverSessionProvider);
}

TEST_F(CollisionDetectionTest, when_backoff_time_is_2_should_be_restarted_the_transmitter_module) {
    collisionDetection.backoffTime = 2u;

    ASSERT_FALSE(LRP_CollisionDetection_shouldRestartTransmitterModule(&collisionDetection));
    ASSERT_EQ(collisionDetection.backoffTime, 1u);
    ASSERT_FALSE(LRP_CollisionDetection_shouldRestartTransmitterModule(&collisionDetection));
    ASSERT_EQ(collisionDetection.backoffTime, 0u);
    ASSERT_TRUE(LRP_CollisionDetection_shouldRestartTransmitterModule(&collisionDetection));
    ASSERT_EQ(collisionDetection.backoffTime, 0u);
}

TEST_F(CollisionDetectionTest, when_receiver_and_transmitter_layer_have_decode_error_isDecodeError_should_return_true) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                           LRP_LINK_LAYER_DECODE_ERROR);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                           LRP_LINK_LAYER_DECODE_ERROR);

    ASSERT_TRUE(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiverSessionProvider));
    ASSERT_TRUE(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider));
}

TEST_F(CollisionDetectionTest,
       when_receiver_layer_has_decode_error_and_is_is_handled_isDecodeError_should_return_true) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                           LRP_LINK_LAYER_DECODE_ERROR);
    LRP_LinkLayer_setSkip((LRPSessionProvider *) collisionDetection.transmitterSessionProvider);

    LRP_CollisionDetection_decodeErrorHandler(&collisionDetection);

    ASSERT_TRUE(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                                      LRP_LINK_LAYER_DECODE_ERROR));
    ASSERT_TRUE(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                                      LRP_LINK_LAYER_DECODE_ERROR));
}

TEST_F(CollisionDetectionTest, when_receiver_and_transmitter_layer_have_not_error_isDecodeError_should_return_false) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                           LRP_LINK_LAYER_NO_ERROR);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                           LRP_LINK_LAYER_NO_ERROR);

    ASSERT_FALSE(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiverSessionProvider));
    ASSERT_FALSE(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider));
}

TEST_F(CollisionDetectionTest,
       when_receiver_and_transmitter_layer_have_noise_stroke_error_isDecodeError_should_return_false) {
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                           LRP_LINK_LAYER_NOISE_STROKE_ERROR);
    LRP_LinkLayer_setError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                           LRP_LINK_LAYER_NOISE_STROKE_ERROR);

    ASSERT_FALSE(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.receiverSessionProvider));
    ASSERT_FALSE(
            LRP_CollisionDetection_isDecodeError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider));
}

TEST_F(CollisionDetectionTest, when_data_is_a_noise_stroke_isNoiseStrokeError_should_return_true) {
    unsigned char data = LRP_COLLISION_DETECTION_NOISE_STROKE;

    ASSERT_TRUE(LRP_CollisionDetection_isNoiseStrokeError(&data));
}

TEST_F(CollisionDetectionTest, when_data_is_not_a_noise_stroke_isNoiseStrokeError_should_return_false) {
    unsigned char data = 0;

    ASSERT_FALSE(LRP_CollisionDetection_isNoiseStrokeError(&data));
}

TEST_F(CollisionDetectionTest, when_noiseStrokeErrorHandler_is_called_set_noise_stroke_error_and_set_backoff_time) {
    LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);

    ASSERT_EQ(collisionDetection.numberOfCollisions, 1);
    ASSERT_TRUE(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.receiverSessionProvider,
                                      LRP_LINK_LAYER_NOISE_STROKE_ERROR));
    ASSERT_TRUE(LRP_LinkLayer_isError((LRPSessionProvider *) collisionDetection.transmitterSessionProvider,
                                      LRP_LINK_LAYER_NOISE_STROKE_ERROR));
}

TEST_F(CollisionDetectionTest,
       when_numberOfCollisions_is_reached_the_maximum_value_isReachedMaxNumberOfCollisions_should_return_true) {
    for (int i = 0; i < maxNumberOfCollisions; ++i) {
        LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);
    }

    ASSERT_EQ(collisionDetection.numberOfCollisions, maxNumberOfCollisions);
    ASSERT_TRUE(LRP_CollisionDetection_isReachedMaxNumberOfCollisions(&collisionDetection));
}

TEST_F(CollisionDetectionTest,
       when_numberOfCollisions_is_bigger_than_the_maximum_value_isReachedMaxNumberOfCollisions_should_return_true) {
    unsigned char maxNumberOfCollisionsPlusOne = maxNumberOfCollisions + 1;
    for (int i = 0; i < maxNumberOfCollisionsPlusOne; ++i) {
        LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);
    }

    ASSERT_EQ(collisionDetection.numberOfCollisions, maxNumberOfCollisions);
    ASSERT_TRUE(LRP_CollisionDetection_isReachedMaxNumberOfCollisions(&collisionDetection));
}

TEST_F(CollisionDetectionTest,
       when_numberOfCollisions_is_not_reached_the_maximum_value_isReachedMaxNumberOfCollisions_should_return_false) {
    unsigned char maxNumberOfCollisionsMinusOne = maxNumberOfCollisions - 1;
    for (int i = 0; i < maxNumberOfCollisionsMinusOne; ++i) {
        LRP_CollisionDetection_noiseStrokeErrorHandler(&collisionDetection);
    }

    ASSERT_EQ(collisionDetection.numberOfCollisions, maxNumberOfCollisionsMinusOne);
    ASSERT_FALSE(LRP_CollisionDetection_isReachedMaxNumberOfCollisions(&collisionDetection));
}