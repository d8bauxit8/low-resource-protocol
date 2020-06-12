#ifndef RECEIVE_TESTING_H
#define RECEIVE_TESTING_H

#include "testing.h"
#include "../src/receive/receive_application_layer.h"
#include "../src/receive/receive_line_code_layer.h"
#include "../src/receive/receive_validator_layer.h"
#include "../src/receive/receive_link_layer.h"

void test_printReceiveData(_LRPFrame *const frame);

void test_printReceiveFrameHeader(_LRPFrame *const frame);

void test_printReceiveFrameStatus(_LRPFrame *const frame);

void test_printReceiveFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength);

void
test_printReceiveLayer(_LRPReceiveSessionProvider *const sessionProvider, const unsigned char const frameBufferLength);

void
test_receiveADataFlow(_LRPSessionProvider *const sessionProvider, unsigned char *data,
                      const unsigned char const dataLength);

unsigned char test_receiveFrameController(_FrameData *const frameData);

#endif // RECEIVE_TESTING_H