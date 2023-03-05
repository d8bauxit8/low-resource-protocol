#include <gtest/gtest.h>
#include "../../../src/transmitter/layers/link.layer.h"
#include "../../../src/transmitter/transmitter.module.h"

class TransmitterLinkLayerTest : public ::testing::Test {
protected:

    LRPTransmitterSessionProvider transmitterSessionProvider{};
    unsigned char sourceDeviceId = 0b10100u;
    const unsigned char targetId = 0b11001u;
    const unsigned char command = 0b101u;
    const unsigned char length = 0b011u;
    LRPFrame frameBuffer[3]{};

    void SetUp() override {
        LRP_Transmitter_init(&transmitterSessionProvider, &sourceDeviceId, frameBuffer, 3u);

        frameBuffer[0].buffer[0] = (unsigned char) (targetId << 3u) | command;
        frameBuffer[0].buffer[1] = (unsigned char) (sourceDeviceId << 3u) | length;
        frameBuffer[0].buffer[2] = 'L';
        frameBuffer[0].buffer[3] = 'R';
        frameBuffer[0].buffer[4] = 'P';

        transmitterSessionProvider.linkCurrentFrame->length = 3u;
        transmitterSessionProvider.linkCurrentFrame->status = LRP_TRANSMITTER_FRAME_READY_TO_TRANSMITTER;
    }

    void TearDown() override {
        ASSERT_EQ(transmitterSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*transmitterSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(transmitterSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, LRP_FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, LRP_FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(transmitterSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(TransmitterLinkLayerTest, buffer_should_be_handled_right) {
    const unsigned char data = LRP_TransmitterLinkLayer_handler(&transmitterSessionProvider);

    ASSERT_EQ(data, frameBuffer[0].buffer[0]);
    ASSERT_EQ(transmitterSessionProvider.indexOfWrittenBytes, 1u);
}

TEST_F(TransmitterLinkLayerTest, the_unwritten_data_should_be_checked_in_buffer) {
    ASSERT_TRUE(LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(&transmitterSessionProvider));

    transmitterSessionProvider.indexOfWrittenBytes = 4u;
    ASSERT_TRUE(LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(&transmitterSessionProvider));

    transmitterSessionProvider.indexOfWrittenBytes = 5u;
    ASSERT_FALSE(LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(&transmitterSessionProvider));
}

TEST_F(TransmitterLinkLayerTest, should_check_the_frame_is_ready_to_transmitting) {
    ASSERT_TRUE(LRP_TransmitterLinkLayer_isReadyToTransmitter(&transmitterSessionProvider));

    transmitterSessionProvider.linkCurrentFrame->status = LRP_FRAME_READY_TO_REDEFINE;
    ASSERT_FALSE(LRP_TransmitterLinkLayer_isReadyToTransmitter(&transmitterSessionProvider));
}

TEST_F(TransmitterLinkLayerTest, transmitting_should_be_started_right) {
    transmitterSessionProvider.indexOfWrittenBytes = 4u;

    LRP_TransmitterLinkLayer_startTransmitting(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.indexOfWrittenBytes, 0u);
    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_OK);
    ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame->status, LRP_TRANSMITTER_FRAME_TRANSMITTING);
}

TEST_F(TransmitterLinkLayerTest, transmitting_should_be_ended_right) {
    transmitterSessionProvider.linkCurrentFrame->status = LRP_TRANSMITTER_FRAME_TRANSMITTING;
    transmitterSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;

    LRP_TransmitterLinkLayer_endTransmitting(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(frameBuffer[0].status, LRP_FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}

TEST_F(TransmitterLinkLayerTest, the_error_status_should_be_handled_right) {
    // There is no error
    transmitterSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_OK);

    // There is error but there is not transmitting
    transmitterSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_ERROR;
    transmitterSessionProvider.linkCurrentFrame->status = LRP_FRAME_READY_TO_REDEFINE;
    LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame->status, LRP_FRAME_READY_TO_REDEFINE);

    // There is error and there is transmitting
    transmitterSessionProvider.linkLayerStatus = LRP_LINK_LAYER_STATUS_ERROR;
    transmitterSessionProvider.linkCurrentFrame->status = LRP_TRANSMITTER_FRAME_TRANSMITTING;
    LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, LRP_LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame->status, LRP_TRANSMITTER_FRAME_READY_TO_TRANSMITTER);
}
