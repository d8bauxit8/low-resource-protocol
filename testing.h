//
// Created by D8bauxit8 on 4/12/2020.
//

#ifndef LOWRESOURCEPROTOCOLLIBRARY_TESTING_H
#define LOWRESOURCEPROTOCOLLIBRARY_TESTING_H

#include "src/controller.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 128 ? '1' : '0'), \
  (byte & 64 ? '1' : '0'), \
  (byte & 32 ? '1' : '0'), \
  (byte & 16 ? '1' : '0'), \
  (byte & 8 ? '1' : '0'), \
  (byte & 4 ? '1' : '0'), \
  (byte & 2 ? '1' : '0'), \
  (byte & 1 ? '1' : '0')

void test_printData(_LRPFrame *const frame);

void test_printFrameStatus(_LRPFrame *const frame);

void test_printFrameHeader(_LRPFrame *const frame);

void test_printReceiveFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength);

void test_printReceiveLayer(_LRPReceiveLayer *const receive);

unsigned char test_generateParity(unsigned char data);

void test_sendData(_LRPReceiveLayer *const receive, unsigned char target, unsigned char source,
                   unsigned char framingError, unsigned char overrunError, char *data);

void test_framingError(void);

void test_overrunError(void);

void test_parityBitError(void);

void describe(char *description, void (*it(void)));

#endif //LOWRESOURCEPROTOCOLLIBRARY_TESTING_H
