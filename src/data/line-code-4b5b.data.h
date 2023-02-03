//
// Created by Jehoda Márk on 2023. 02. 03..
//

#ifndef LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_DATA_H
#define LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_DATA_H

#ifdef    __cplusplus
extern "C" {
#endif

#define LINE_CODE_4B5B_SUCCESSFUL_DECODING 0
#define LINE_CODE_4B5B_FAILED_DECODING 1

typedef enum {
    SuccessfulDecoding = LINE_CODE_4B5B_SUCCESSFUL_DECODING, FailedDecoding = LINE_CODE_4B5B_FAILED_DECODING
} LRPLineCode4B5BDecodingState;

// Recommend control codes
// https://en.wikipedia.org/wiki/4B5B
#define LINE_CODE_4B5B_START_DELIMITER_BYTE 0b11000000
#define LINE_CODE_4B5B_STOP_DELIMITER_BYTE 0b01101000

typedef struct LRPLineCode4B5B {
    unsigned char index;
    unsigned char buffer[2];
} LRPLineCode4B5B;

#ifdef    __cplusplus
}
#endif

#endif //LOW_RESOURCE_PROTOCOL_LINE_CODE_4B5B_DATA_H
