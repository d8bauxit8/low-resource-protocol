#include <stdio.h>
#include "testing.h"
//
// Created by D8bauxit8 on 4/12/2020.
//

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