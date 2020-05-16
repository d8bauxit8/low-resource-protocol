//
// Created by D8bauxit8 on 4/12/2020.
//

#ifndef LOWRESOURCEPROTOCOLLIBRARY_TESTING_H
#define LOWRESOURCEPROTOCOLLIBRARY_TESTING_H

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

#include "src/receive/receive_application_layer.h"
#include "src/receive/receive_line_code_layer.h"
#include "src/receive/receive_validator_layer.h"
#include "src/receive/receive_link_layer.h"

void test_printData(_LRPFrame *const frame);

void test_printFrameStatus(_LRPFrame *const frame);

void test_printFrameHeader(_LRPFrame *const frame);

void test_printReceiveFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength);

void test_printReceiveLayer(_LRPSessionProvider *const receive, const unsigned char const frameBufferLength);

void test_readData(_LRPSessionProvider *const receive, unsigned char *data, const unsigned char const dataLength);

unsigned char test_receiveFrameController(_FrameData *const frameData);

void describe(char *description, void (*it(void)));

#endif //LOWRESOURCEPROTOCOLLIBRARY_TESTING_H
