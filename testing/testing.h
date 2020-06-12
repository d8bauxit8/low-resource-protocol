//
// Created by D8bauxit8 on 4/12/2020.
//

#ifndef TESTING_H
#define TESTING_H

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

#define TEN_BITS_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c"
#define TEN_BITS_TO_BINARY(byte)  \
  (byte & 512 ? '1' : '0'), \
  (byte & 256 ? '1' : '0'), \
  (byte & 128 ? '1' : '0'), \
  (byte & 64 ? '1' : '0'), \
  (byte & 32 ? '1' : '0'), \
  (byte & 16 ? '1' : '0'), \
  (byte & 8 ? '1' : '0'), \
  (byte & 4 ? '1' : '0'), \
  (byte & 2 ? '1' : '0'), \
  (byte & 1 ? '1' : '0')

#include "../src/session_provider.h"

#define FRAME_BUFFER_LENGTH 3

#define R_BUFFER_TEST_DATA_LENGTH 10
extern unsigned char r_bufferTest[];

#define R_BUFFER_NOPE_DATA_LENGTH 10
extern unsigned char r_bufferNope[];

#define T_BUFFER_TEST_DATA_LENGTH 4
extern unsigned char t_bufferTest[];

#define T_BUFFER_NOPE_DATA_LENGTH 4
extern unsigned char t_bufferNope[];

extern const unsigned char const sourceDeviceId;
extern const unsigned char const targetDeviceId;
extern const unsigned char command;

void test_printLinkLayerStatus(_LRPSessionProvider *const sessionProvider);

void test(char *description, void (*testCase(void)));

#endif // TESTING_H
