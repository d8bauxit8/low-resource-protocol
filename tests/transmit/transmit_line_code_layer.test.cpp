#include <gtest/gtest.h>
#include "../../src/transmit/transmit_line_code_layer.h"

class TransmitLineCodeLayerTest : public ::testing::Test {
protected:

    LRPTransmitSessionProvider transmitSessionProvider{};
    const unsigned char sourceDeviceId = 0b10100;
    LRPFrame frameBuffer[3]{};

    LRPLineCode4B5B lineCode4B5B{};
    unsigned char buffer[2]{};

    void SetUp() override {
        LRP_SessionProvider_init((LRPSessionProvider *) &transmitSessionProvider, &sourceDeviceId, frameBuffer, 3);

        frameBuffer[0].buffer[0] = 0b11001101;
        frameBuffer[0].buffer[1] = 0b10100011;
        frameBuffer[0].buffer[2] = 'L';
        frameBuffer[0].buffer[3] = 'R';
        frameBuffer[0].buffer[4] = 'P';

        transmitSessionProvider.linkCurrentFrame->length = 3;
        transmitSessionProvider.linkCurrentFrame->status = TRANSMIT_FRAME_READY_TO_TRANSMIT;
        transmitSessionProvider.linkLayerStatus = LINK_LAYER_STATUS_SKIP;

        lineCode4B5B.index = 0;
        buffer[0] = 0;
        buffer[0] = 0;
        lineCode4B5B.buffer[0] = &buffer[0];
        lineCode4B5B.buffer[1] = &buffer[1];
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
    ASSERT_EQ(transmitSessionProvider.indexOfWroteBytes, 0);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x5B);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x57);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0xAB);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x15);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x5D);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x7E);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, 0x1);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_OK);

    LRP_TransmitLineCodeLayer_handler(&transmitSessionProvider, &lineCode4B5B, &data);
    ASSERT_EQ(data, LINE_CODE_4B5B_STOP_DELIMITER_BYTE);
    ASSERT_EQ(transmitSessionProvider.linkLayerStatus, LINK_LAYER_STATUS_SKIP);
    ASSERT_EQ(frameBuffer[0].status, FRAME_READY_TO_REDEFINE);
    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame, &frameBuffer[1]);
}

TEST_F(TransmitLineCodeLayerTest, Should_Be_Started_Transmitting) {
    lineCode4B5B.index = 4;
    *lineCode4B5B.buffer[0] = 0xFF;
    *lineCode4B5B.buffer[1] = 0xFE;

    unsigned char data;
    LRP_TransmitLineCodeLayer_startTransmitting(&transmitSessionProvider, &lineCode4B5B, &data);

    ASSERT_EQ(transmitSessionProvider.linkCurrentFrame->status, TRANSMIT_FRAME_TRANSMITTING);
    ASSERT_EQ(lineCode4B5B.index, 0);
    ASSERT_EQ(*lineCode4B5B.buffer[0], 0);
    ASSERT_EQ(*lineCode4B5B.buffer[1], 0);
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