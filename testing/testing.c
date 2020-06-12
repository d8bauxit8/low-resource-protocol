#include <stdio.h>
#include "testing.h"
//
// Created by D8bauxit8 on 4/12/2020.
//

unsigned char r_bufferTest[] = {0b00011000, 0b01110111, 0b00101010, 0b10100111, 0b11010110, 0b01010010,
                                0b01110101, 0b10101001, 0b00000101,
                                0b00001101};

unsigned char r_bufferNope[] = {0b00011000, 0b01110111, 0b00101010, 0b11000111, 0b01010101, 0b01110111,
                                0b11111110, 0b00101101, 0b00000111,
                                0b00001101};

unsigned char t_bufferTest[] = "TEST";

unsigned char t_bufferNope[] = "NOPE";

const unsigned char const sourceDeviceId = 0b00001100;
const unsigned char const targetDeviceId = 0b00010011;
const unsigned char command = 0b00000011;

void test_printLinkLayerStatus(_LRPSessionProvider *const sessionProvider) {
    printf("\n\tLink layer status: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(sessionProvider->linkLayerStatus));
    switch (sessionProvider->linkLayerStatus) {
        case LINK_LAYER_STATUS_OK:
            printf("\n\t\tOK");
            break;
        case LINK_LAYER_STATUS_SKIP:
            printf("\n\t\tSKIP");
            break;
    }
}

void test(char *description, void (*testCase(void))) {
    printf("\n\n##########  Start '%s' test case.  ##########", description);
    testCase();
    printf("\n##########  End of '%s' test case.  ##########", description);
}