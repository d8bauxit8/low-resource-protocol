#include <stdio.h>
#include "receive-testing.h"

void test_printReceiveData(_LRPFrame *const frame) {
    if (frame->status != RECEIVE_FRAME_READY_TO_READ) {
        printf("\n\t\tData: undefined");
        return;
    }
    printf("\n\t\tData: ");
    for (unsigned char i = 0; i < frame->length; i++) {
        printf("%c", *frame->data[i]);
    }
}

void test_printReceiveFrameHeader(_LRPFrame *const frame) {
    if (frame->status == RECEIVE_FRAME_READY_TO_READ) {
        printf("\n\t\tTarget device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->targetDeviceId));
        printf("\n\t\tCommand: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->command));
        printf("\n\t\tSource device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->sourceDeviceId));
        printf("\n\t\tLength: %u", frame->length);
    } else {
        printf("\n\t\tTarget device ID: undefined");
        printf("\n\t\tCommand: undefined");
        printf("\n\t\tSource device ID: undefined");
        printf("\n\t\tLength: undefined");
    }
    if (frame->status != FRAME_READY_TO_REDEFINE) {
        printf("\n\t\tBuffer: %s", frame->buffer);
    } else {
        printf("\n\t\tBuffer: undefined");
    }
}

void test_printReceiveFrameStatus(_LRPFrame *const frame) {
    printf("\n\t\tStatus: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->status));

    switch (frame->status) {
        case RECEIVE_FRAME_IN_RECEIVING:
            printf("\n\t\t\tIN RECEIVING");
            break;
        case RECEIVE_FRAME_READY_TO_CHECK:
            printf("\n\t\t\tREADY TO CHECK");
            break;
        case RECEIVE_FRAME_READY_TO_READ:
            printf("\n\t\t\tREADY TO READ");
            break;
        case FRAME_READY_TO_REDEFINE:
            printf("\n\t\t\tREADY TO REDEFINE");
            break;
    }
}

void test_printReceiveFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength) {
    for (char i = 0; i < frameBufferLength; i++) {
        test_printReceiveFrameHeader(&frameBuffer[i]);
        test_printReceiveFrameStatus(&frameBuffer[i]);
        test_printReceiveData(&frameBuffer[i]);
        printf("\n\t\t--------------------------");
    }
}

void
test_printReceiveLayer(_LRPReceiveSessionProvider *const sessionProvider, const unsigned char const frameBufferLength) {
    printf("\nReceive layer:");
    test_printLinkLayerStatus(sessionProvider);
    printf("\n\tFrame buffer length: %u", frameBufferLength);
    if (sessionProvider->indexOfReadBytes != 204) {
        printf("\n\tIndex of read byte: %u", sessionProvider->indexOfReadBytes);
    } else {
        printf("\n\tIndex of read byte: undefined");
    }
    printf("\n\tReceive device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*sessionProvider->deviceId));
    printf("\n\tFrame list:");
    test_printReceiveFrameList(sessionProvider->frameBuffer, frameBufferLength);
    printf("\n###################################################");
}

unsigned char test_receiveFrameController(_FrameData *const frameData) {
    printf("\nReceive frame controller:");
    printf("\n\tTarget device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frameData->targetDeviceId));
    printf("\n\tSource device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frameData->sourceDeviceId));
    printf("\n\tCommand: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frameData->command));
    printf("\n\tLength: %u", frameData->length);
    printf("\n\tData: ");
    for (unsigned char i = 0; i < frameData->length; i++) {
        printf("%c", *frameData->data[i]);
    }
    return 1;
}

void
test_receiveADataFlow(_LRPSessionProvider *const sessionProvider, unsigned char *data,
                      const unsigned char const dataLength) {
    _LRPLineCode4B5B lineCode4B5B;
    unsigned char buffer[2];
    lineCode4B5B.buffer[0] = &buffer[0];
    lineCode4B5B.buffer[1] = &buffer[1];

    for (char i = 0; i < dataLength; i++) {
        LRP_ReceiveLineCodeLayer_handler((_LRPReceiveSessionProvider *) sessionProvider, &lineCode4B5B, &data[i]);
    }
}