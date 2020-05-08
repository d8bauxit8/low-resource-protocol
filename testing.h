//
// Created by D8bauxit8 on 4/12/2020.
//

#ifndef LOWRESOURCEPROTOCOLLIBRARY_TESTING_H
#define LOWRESOURCEPROTOCOLLIBRARY_TESTING_H

#include "src/receive/receive_application_layer.h"
#include "src/receive/receive_line_code_layer.h"
#include "src/receive/receive_validator_layer.h"
#include "src/receive/receive_link_layer.h"

void test_printData(_LRPFrame *const frame);

void test_printFrameStatus(_LRPFrame *const frame);

void test_printFrameHeader(_LRPFrame *const frame);

void test_printReceiveFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength);

void test_printReceiveLayer(_LRPReceiveLayer *const receive, const unsigned char const frameBufferLength);

void test_sendData(_LRPReceiveLayer *const receive, unsigned char *data, const unsigned char const dataLength);

unsigned char test_receiveFrameController(_FrameData *const frameData);

void describe(char *description, void (*it(void)));

#endif //LOWRESOURCEPROTOCOLLIBRARY_TESTING_H
