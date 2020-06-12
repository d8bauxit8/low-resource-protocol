#include <stdio.h>
#include "testing.h"
//
// Created by D8bauxit8 on 4/12/2020.
//

void test(char *description, void (*testCase(void))) {
    printf("\n\n##########  Start '%s' test case.  ##########", description);
    testCase();
    printf("\n##########  End of '%s' test case.  ##########", description);
}