#ifndef TRANSMIT_TESTING_H
#define TRANSMIT_TESTING_H

#include "testing.h"
#include "../src/transmit/transmit_application_layer.h"
#include "../src/transmit/transmit_line_code_layer.h"
#include "../src/transmit/transmit_validator_layer.h"
#include "../src/transmit/transmit_link_layer.h"

void test_printTransmitData(_LRPFrame *const frame);

void test_printTransmitFrameHeader(_LRPFrame *const frame);

void test_printTransmitFrameStatus(_LRPFrame *const frame);

void test_printTransmitFrameList(_LRPFrame *const frameBuffer, const unsigned char const frameBufferLength);

void test_printTransmitLayer(_LRPTransmitSessionProvider *const sessionProvider,
                             const unsigned char const frameBufferLength);

#endif // TRANSMIT_TESTING_H