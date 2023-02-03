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
        transmitterSessionProvider.linkCurrentFrame->status = TRANSMITTER_FRAME_READY_TO_TRANSMITTER;
    }

    void TearDown() override {
        ASSERT_EQ(transmitterSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*transmitterSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(transmitterSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(transmitterSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(transmitterSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(TransmitterLinkLayerTest, Should_Be_Handled_Buffer) {
    const unsigned char data = LRP_TransmitterLinkLayer_handler(&transmitterSessionProvider);

    ASSERT_EQ(data, frameBuffer[0].buffer[0]);
    ASSERT_EQ(transmitterSessionProvider.indexOfWrittenBytes, 1u);
}

TEST_F(TransmitterLinkLayerTest, Should_Be_Checked_The_Unwritten_Data_In_Buffer) {
    ASSERT_TRUE(LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(&transmitterSessionProvider));

    transmitterSessionProvider.indexOfWrittenBytes = 4u;
    ASSERT_TRUE(LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(&transmitterSessionProvider));

    transmitterSessionProvider.indexOfWrittenBytes = 5u;
    ASSERT_FALSE(LRP_TransmitterLinkLayer_isUnwrittenDataInBuffer(&transmitterSessionProvider));
}

TEST_F(TransmitterLinkLayerTest, Should_Be_Checked_The_Frame_Is_Ready_To_Transmitter) {
    ASSERT_TRUE(LRP_TransmitterLinkLayer_isReadyToTransmitter(&transmitterSessionProvider));

    transmitterSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;
    ASSERT_FALSE(LRP_TransmitterLinkLayer_isReadyToTransmitter(&transmitterSessionProvider));
}

TEST_F(TransmitterLinkLayerTest, Should_Be_Started_Transmitting) {
    transmitterSessionProvider.indexOfWrittenBytes = 4u;

    LRP_TransmitterLinkLayer_startTransmitting(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.indexOfWrittenBytes, 0u);
    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, OK);
    ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame->status, TRANSMITTER_FRAME_TRANSMITTERTING);
}

TEST_F(TransmitterLinkLayerTest, Should_Be_Ended_Transmitting) {
    transmitterSessionProvider.linkCurrentFrame->status = TRANSMITTER_FRAME_TRANSMITTERTING;
    transmitterSessionProvider.linkLayerStatus = OK;

    LRP_TransmitterLinkLayer_endTransmitting(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}

TEST_F(TransmitterLinkLayerTest, Should_Be_Handled_The_Error_Status) {
    // There is no error
    transmitterSessionProvider.linkLayerStatus = OK;
    LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, OK);

    // There is error but there is not transmitting
    transmitterSessionProvider.linkLayerStatus = Error;
    transmitterSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;
    LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame->status, FRAME_READY_TO_REDEFINE);

    // There is error and there is transmitting
    transmitterSessionProvider.linkLayerStatus = Error;
    transmitterSessionProvider.linkCurrentFrame->status = TRANSMITTER_FRAME_TRANSMITTERTING;
    LRP_TransmitterLinkLayer_errorStatusHandler(&transmitterSessionProvider);

    ASSERT_EQ(transmitterSessionProvider.linkLayerStatus, Skip);
    ASSERT_EQ(transmitterSessionProvider.linkCurrentFrame->status, TRANSMITTER_FRAME_READY_TO_TRANSMITTER);
}
