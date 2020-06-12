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

void test(char *description, void (*testCase(void)));

#endif // TESTING_H
