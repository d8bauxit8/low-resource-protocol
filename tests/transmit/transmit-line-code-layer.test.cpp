#include <gtest/gtest.h>
#include "../../src/transmit/transmit-line-code-layer.h"
#include "../../src/transmit/transmit-session-provider.h"

class TransmitLineCodeLayerTest : public ::testing::Test {
protected:

    LRPTransmitSessionProvider transmitSessionProvider{};
    const unsigned char sourceDeviceId = 0b10100u;
    const unsigned char targetId = 0b11001u;
    const unsigned char command = 0b101u;
    const unsigned char length = 0b011u;
    LRPFrame frameBuffer[3]{};

    LRPLineCode4B5B lineCode4B5B{};

    void SetUp() override {
        LRP_TransmitSessionProvider_init(&transmitSessionProvider, &sourceDeviceId, frameBuffer, 3u);

        frameBuffer[0].buffer[0] = (unsigned char) (targetId << 3u) | command;
        frameBuffer[0].buffer[1] = (unsigned char) (sourceDeviceId << 3u) | length;
        frameBuffer[0].buffer[2] = 'L';
        frameBuffer[0].buffer[3] = 'R';
        frameBuffer[0].buffer[4] = 'P';

        transmitSessionProvider.linkCurrentFrame->length = 3u;
        transmitSessionProvider.linkCurrentFrame->status = TRANSMIT_FRAME_READY_TO_TRANSMIT;
        transmitSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;

        lineCode4B5B.index = 0u;
        lineCode4B5B.buffer[0] = 0u;
        lineCode4B5B.buffer[1] = 0u;
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

TEST_F(TransmitLineCodeLayerTest, Should_Be_Handled_Frame_Transmitting) {
    unsigned char data;

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, LINE_CODE_4B5B_START_DELIMITER_BYTE);
    ASSERT_EQ(transmitSessionProvider.indexOfWrittenBytes, 0u);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x5Bu);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x57u);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0xABu);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x15u);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x5Du);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x7Eu);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x1u);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, LINE_CODE_4B5B_STOP_DELIMITER_BYTE);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}

TEST_F(TransmitLineCodeLayerTest, Should_Be_Started_Transmitting) {
    lineCode4B5B.index = 4u;
    lineCode4B5B.buffer[0] = 0xFFu;
    lineCode4B5B.buffer[1] = 0xFEu;

    unsigned char data;
    LRP_TransmitLineCodeLayer_startTransmitting(&transmitSessionProvider, &lineCode4B5B, &data);

    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(lineCode4B5B.index, 0u);
    ASSERT_EQ(lineCode4B5B.buffer[0], 0u);
    ASSERT_EQ(lineCode4B5B.buffer[1], 0u);
    ASSERT_EQ(data, LINE_CODE_4B5B_START_DELIMITER_BYTE);
}

TEST_F(TransmitLineCodeLayerTest, Should_Be_Checked_The_Frame_Is_Ready_To_Transmitting) {
    ASSERT_TRUE(LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(&transmitSessionProvider));

    transmitSessionProvider.linkCurrentFrame->status = FRAME_READY_TO_REDEFINE;
    transmitSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    ASSERT_FALSE(LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(&transmitSessionProvider));

    transmitSessionProvider.linkCurrentFrame->status = TRANSMIT_FRAME_READY_TO_TRANSMIT;
    transmitSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_ERROR;
    ASSERT_FALSE(LRP_TransmitLineCodeLayer_isReadyToStartTransmitting(&transmitSessionProvider));
}