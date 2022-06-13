#include <gtest/gtest.h>
#include "../../src/transmit/transmit-link-layer.h"
#include "../../src/transmit/transmit-session-provider.h"

class TransmitLinkLayerTest : public ::testing::Test {
protected:

    LRPTransmitSessionProvider transmitSessionProvider{};
    const unsigned char sourceDeviceId = 0b10100u;
    const unsigned char targetId = 0b11001u;
    const unsigned char command = 0b101u;
    const unsigned char length = 0b011u;
    LRPFrame frameBuffer[3]{};

    void SetUp() override {
        LRP_TransmitSessionProvider_init(&transmitSessionProvider, &sourceDeviceId, frameBuffer, 3u);

        frameBuffer[0].buffer[0] = (unsigned char) (targetId << 3u) | command;
        frameBuffer[0].buffer[1] = (unsigned char) (sourceDeviceId << 3u) | length;
        frameBuffer[0].buffer[2] = 'L';
        frameBuffer[0].buffer[3] = 'R';
        frameBuffer[0].buffer[4] = 'P';

        transmitSessionProvider.linkCurrentFrame->length = 3u;
        transmitSessionProvider.linkCurrentFrame->status = TRANSMIT_FRAME_READY_TO_TRANSMIT;
    }

    void TearDown() override {
        ASSERT_EQ(transmitSessionProvider.deviceId, &sourceDeviceId);
        ASSERT_EQ(*transmitSessionProvider.deviceId, sourceDeviceId);

        ASSERT_EQ(transmitSessionProvider.frameBuffer, frameBuffer);

        ASSERT_EQ(frameBuffer[1].status, FRAME_READY_TO_REDEFINE);
        ASSERT_EQ(frameBuffer[2].status, FRAME_READY_TO_REDEFINE);

        ASSERT_EQ(transmitSessionProvider.applicationCurrentFrame, &frameBuffer[0]);
        ASSERT_EQ(transmitSessionProvider.validatorCurrentFrame, &frameBuffer[0]);
    }
};

TEST_F(TransmitLinkLayerTest, Should_Be_Handled_Buffer) {
    const unsigned char data = LRP_TransmitLinkLayer_handler(&transmitSessionProvider);

    ASSERT_EQ(data, frameBuffer[0].buffer[0]);
    ASSERT_EQ(transmitSessionProvider.indexOfWrittenBytes, 1u);
}

TEST_F(TransmitLinkLayerTest, Should_Be_Checked_The_Unwritten_Data_In_Buffer) {
    ASSERT_TRUE(LRP_TransmitLinkLayer_isUnwrittenDataInBuffer(&transmitSessionProvider));

    transmitSessionProvider.indexOfWrittenBytes = 4u;
    ASSERT_TRUE(LRP_TransmitLinkLayer_isUnwrittenDataInBuffer(&transmitSessionProvider));

    transmitSessionProvider.indexOfWrittenBytes = 5u;
    ASSERT_FALSE(LRP_TransmitLinkLayer_isUnwrittenDataInBuffer(&transmitSessionProvider));
}

TEST_F(TransmitLinkLayerTest, Should_Be_Checked_The_Frame_Is_Ready_To_Transmit) {
    ASSERT_TRUE(LRP_TransmitLinkLayer_isReadyToTransmit(&transmitSessionProvider));

    transmitSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;
    ASSERT_FALSE(LRP_TransmitLinkLayer_isReadyToTransmit(&transmitSessionProvider));
}

TEST_F(TransmitLinkLayerTest, Should_Be_Started_Transmitting) {
    transmitSessionProvider.indexOfWrittenBytes = 4u;

    LRP_TransmitLinkLayer_startTransmitting(&transmitSessionProvider);

    ASSERT_EQ(transmitSessionProvider.indexOfWrittenBytes, 0u);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
}

TEST_F(TransmitLinkLayerTest, Should_Be_Ended_Transmitting) {
    transmitSessionProvider.linkCurrentFrame->status = TRANSMIT_FRAME_TRANSMITTING;
    transmitSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;

    LRP_TransmitLinkLayer_endTransmitting(&transmitSessionProvider);

    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}

TEST_F(TransmitLinkLayerTest, Should_Be_Handled_The_Error_Status) {
    // There is no error
    transmitSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_OK;
    LRP_TransmitLinkLayer_errorStatusHandler(&transmitSessionProvider);

    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    // There is error but there is not transmitting
    transmitSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_ERROR;
    transmitSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;
    LRP_TransmitLinkLayer_errorStatusHandler(&transmitSessionProvider);

    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, FRAME_READY_TO_REDEFINE);

    // There is error and there is transmitting
    transmitSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_ERROR;
    transmitSessionProvider.linkCurrentFrame->status = TRANSMIT_FRAME_TRANSMITTING;
    LRP_TransmitLinkLayer_errorStatusHandler(&transmitSessionProvider);

    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_READY_TO_TRANSMIT);
}
