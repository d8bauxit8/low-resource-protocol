#include <stdio.h>
#include "testing.h"
//
// Created by D8bauxit8 on 4/12/2020.
//

void test_printData(_LRPFrame *const frame) {
    const unsigned char frameBufferLength = sizeof(frame->data);
    printf("\n\t\tData: ");
    for (unsigned char i = 0; i < frameBufferLength; i++) {
        printf("%c", frame->data[i]);
    }
}

void test_printFrameStatus(_LRPFrame *const frame) {
    printf("\n\t\tStatus: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->status));

    switch (frame->status) {
        case FRAME_IN_PROGRESS:
            printf("\n\t\t\tIN PROGRESS");
            break;
        case RECEIVE_FRAME_COMPLETED:
            printf("\n\t\t\tCOMPLETED");
            break;
        case FRAME_READY_TO_REDEFINE:
            printf("\n\t\t\tREADY TO REDEFINE");
            break;
    }
}

void test_printFrameHeader(_LRPFrame *const frame) {
    printf("\n\t\tTarget device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->targetDeviceId));
    printf("\n\t\tCommand: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->command));
    printf("\n\t\tSource device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(frame->sourceDeviceId));
}


void test_printReceiveFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength) {
    for (char i = 0; i < frameBufferLength; i++) {
        test_printFrameHeader(&frameBuffer[i]);
        test_printFrameStatus(&frameBuffer[i]);
        test_printData(&frameBuffer[i]);
    }
}

void test_printReceiveLayer(_LRPReceiveLayer *const receive, const unsigned char const frameBufferLength) {
    printf("\nReceive layer:");
    printf("\n\tNumber of read bytes: %u", receive->numberOfReadBytes);
    printf("\n\tStatus: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(receive->status));
    printf("\n\tFrame buffer length: %u", frameBufferLength);
    printf("\n\tReceive device ID: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*receive->receiveDeviceId));
    printf("\n\tFrame list:");
    test_printReceiveFrameList(receive->frameBuffer, frameBufferLength);
    printf("\n###################################################");
}

unsigned char test_generateParity(unsigned char data) {
    unsigned char parity = 1;

    while (data) {
        parity ^= data & 1;
        data >>= 1;
    }
    return (parity);
}

void test_sendData(_LRPReceiveLayer *const receive, unsigned char target, unsigned char source,
                   unsigned char framingError, unsigned char overrunError, char *data) {

    unsigned char parityBit = test_generateParity(target);
    LRP_receiveLayerHandler(receive, target, &parityBit, &framingError, &overrunError);

    parityBit = test_generateParity(source);
    LRP_receiveLayerHandler(receive, source, &parityBit, &framingError, &overrunError);

    char len = sizeof(data);
    for (char i = 0; i < len; i++) {
        parityBit = test_generateParity(data[i]);
        LRP_receiveLayerHandler(receive, data[i], &parityBit, &framingError, &overrunError);
    }
}

void test_framingError(void) {
    printf("\n\t\t\tRECEIVE FRAMING ERROR");
}

void test_overrunError(void) {
    printf("\n\t\t\tRECEIVE OVERRUN ERROR");
}

void test_parityBitError(void) {
    printf("\n\t\t\tRECEIVE PARITY BIT ERROR");
}

void describe(char *description, void (*it(void))) {
    printf("\n\n##########  Start '%s' test case.  ##########", description);
    it();
    printf("\n##########  End of '%s' test case.  ##########", description);
}