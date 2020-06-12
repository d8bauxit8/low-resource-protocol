#include "stdio.h"
#include "transmit-testing.h"

void test_printTransmitData(_LRPFrame *const frame) {
    if (frame->status == FRAME_READY_TO_REDEFINE) {
        printf("\n\t\tData: undefined");
        return;
    }
    printf("\n\t\tData: ");
    for (unsigned char i = 0; i < frame->length; i++) {
        printf("%c", *frame->data[i]);
    }
}

void test_printTransmitFrameHeader(_LRPFrame *const frame) {
    if (frame->status != FRAME_READY_TO_REDEFINE) {
        printf("\n\t\tTarget device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->targetDeviceId));
        printf("\n\t\tCommand: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->command));
        printf("\n\t\tSource device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->sourceDeviceId));
        printf("\n\t\tLength: %u", frame->length);
        printf("\n\t\tBuffer: %s", frame->buffer);
    } else {
        printf("\n\t\tTarget device ID: undefined");
        printf("\n\t\tCommand: undefined");
        printf("\n\t\tSource device ID: undefined");
        printf("\n\t\tLength: undefined");
        printf("\n\t\tBuffer: undefined");
    }
}

void test_printTransmitFrameStatus(_LRPFrame *const frame) {
    printf("\n\t\tStatus: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->status));

    switch (frame->status) {
        case TRANSMIT_FRAME_RESERVED:
            printf("\n\t\t\tRESERVED");
            break;
        case TRANSMIT_FRAME_READY_TO_CHECK:
            printf("\n\t\t\tREADY TO CHECK");
            break;
        case TRANSMIT_FRAME_READY_TO_TRANSMIT:
            printf("\n\t\t\tREADY TO TRANSMIT");
            break;
        case TRANSMIT_FRAME_TRANSMITTING:
            printf("\n\t\t\tTRANSMITTING");
            break;
        case FRAME_READY_TO_REDEFINE:
            printf("\n\t\t\tREADY TO REDEFINE");
            break;
    }
}

void test_printTransmitFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength) {
    for (char i = 0; i < frameBufferLength; i++) {
        test_printTransmitFrameHeader(&frameBuffer[i]);
        test_printTransmitFrameStatus(&frameBuffer[i]);
        test_printTransmitData(&frameBuffer[i]);
        printf("\n\t\t--------------------------");
    }
}

void test_printTransmitLayer(_LRPTransmitSessionProvider *const sessionProvider,
                             const unsigned char const frameBufferLength) {
    printf("\nTransmit layer:");
    test_printLinkLayerStatus(sessionProvider);
    printf("\n\tFrame buffer length: %u", frameBufferLength);
    if (sessionProvider->indexOfWroteBytes != 204) {
        printf("\n\tIndex of wrote byte: %u", sessionProvider->indexOfWroteBytes);
    } else {
        printf("\n\tIndex of wrote byte: undefined");
    }
    printf("\n\tTransmit device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*sessionProvider->deviceId));
    printf("\n\tFrame list:");
    test_printTransmitFrameList(sessionProvider->frameBuffer, frameBufferLength);
    printf("\n###################################################");
}

void test_printCheckTransmitDataValidity(const unsigned char *const data) {
    for (unsigned char i = 0; i < R_BUFFER_TEST_DATA_LENGTH; i++) {
        if (r_bufferTest[i] != data[i]) {
            printf("\n\tThe index of %u data is invalid", i);
        }
    }
}