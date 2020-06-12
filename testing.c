#include <stdio.h>
#include "testing.h"
//
// Created by D8bauxit8 on 4/12/2020.
//

void test_printData(_LRPFrame *const frame) {
    if (frame->status != RECEIVE_FRAME_READY_TO_READ) {
        printf("\n\t\tData: undefined");
        return;
    }
    printf("\n\t\tData: ");
    for (unsigned char i = 0; i < frame->length; i++) {
        printf("%c", *frame->data[i]);
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

void test_printFrameHeader(_LRPFrame *const frame) {
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


void test_printReceiveFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength) {
    for (char i = 0; i < frameBufferLength; i++) {
        test_printFrameHeader(&frameBuffer[i]);
        test_printReceiveFrameStatus(&frameBuffer[i]);
        test_printData(&frameBuffer[i]);
        printf("\n\t\t--------------------------");
    }
}

void test_printTransmitFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength) {
    for (char i = 0; i < frameBufferLength; i++) {
        test_printFrameHeader(&frameBuffer[i]);
        test_printTransmitFrameStatus(&frameBuffer[i]);
        test_printData(&frameBuffer[i]);
        printf("\n\t\t--------------------------");
    }
}

void test_printReceiveLayer(_LRPSessionProvider *const sessionProvider, const unsigned char const frameBufferLength) {
    printf("\nReceive layer:");
    printf("\n\tStatus: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(sessionProvider->linkLayerStatus));
    printf("\n\tFrame buffer length: %u", frameBufferLength);
    printf("\n\tReceive device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*sessionProvider->deviceId));
    printf("\n\tFrame list:");
    test_printReceiveFrameList(sessionProvider->frameBuffer, frameBufferLength);
    printf("\n###################################################");
}

void test_printTransmitLayer(_LRPSessionProvider *const sessionProvider, const unsigned char const frameBufferLength) {
    printf("\nTransmit layer:");
    printf("\n\tStatus: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(sessionProvider->linkLayerStatus));
    printf("\n\tFrame buffer length: %u", frameBufferLength);
    printf("\n\tReceive device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*sessionProvider->deviceId));
    printf("\n\tFrame list:");
    test_printTransmitFrameList(sessionProvider->frameBuffer, frameBufferLength);
    printf("\n###################################################");
}

void
test_readData(_LRPSessionProvider *const sessionProvider, unsigned char *data, const unsigned char const dataLength) {
    _LRPLineCode4B5B lineCode4B5B;
    unsigned char buffer[2];
    lineCode4B5B.buffer[0] = &buffer[0];
    lineCode4B5B.buffer[1] = &buffer[1];

    for (char i = 0; i < dataLength; i++) {
        LRP_ReceiveLineCodeLayer_handler((_LRPReceiveSessionProvider *) sessionProvider, &lineCode4B5B, &data[i]);
    }
}

void
test_writeData(_LRPSessionProvider *const sessionProvider, unsigned char *data, const unsigned char const dataLength) {
    _LRPLineCode4B5B lineCode4B5B;
    unsigned char buffer[2];
    lineCode4B5B.buffer[0] = &buffer[0];
    lineCode4B5B.buffer[1] = &buffer[1];

    for (char i = 0; i < dataLength; i++) {
        LRP_ReceiveLineCodeLayer_handler((_LRPReceiveSessionProvider *) sessionProvider, &lineCode4B5B, &data[i]);
    }
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

void describe(char *description, void (*it(void))) {
    printf("\n\n##########  Start '%s' test case.  ##########", description);
    it();
    printf("\n##########  End of '%s' test case.  ##########", description);
}