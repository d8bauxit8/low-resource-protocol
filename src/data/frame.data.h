//
// Created by Jehoda Márk on 2023. 02. 03..
//

#ifndef LOW_RESOURCE_PROTOCOL_FRAME_DATA_H
#define LOW_RESOURCE_PROTOCOL_FRAME_DATA_H

#ifdef    __cplusplus
extern "C" {
#endif

#define FRAME_NUMBER_OF_MAX_DATA_BYTES 7u
#define FRAME_NUMBER_OF_HEADER_BYTES 2u
#define FRAME_NUMBER_OF_FRAME_BYTES (FRAME_NUMBER_OF_HEADER_BYTES + FRAME_NUMBER_OF_MAX_DATA_BYTES)

#define FRAME_NO_COMMAND 0b000u
#define FRAME_GROUP_ID_COMMAND 0b111u

typedef enum {
    NoCommand = FRAME_NO_COMMAND, GroupIdCommand = FRAME_GROUP_ID_COMMAND
} LRPFrameCommand;

typedef struct LRPFrameData {
    unsigned char sourceDeviceId: 5;
    unsigned char targetId: 5;
    LRPFrameCommand command: 3;
    unsigned char length: 3;
    unsigned char *data[FRAME_NUMBER_OF_MAX_DATA_BYTES];
} LRPFrameData;

typedef struct LRPFrame {
    unsigned char sourceDeviceId: 5;
    unsigned char targetId: 5;
    LRPFrameCommand command: 3;
    unsigned char length: 3;
    unsigned char *data[FRAME_NUMBER_OF_MAX_DATA_BYTES];
    unsigned char status;
    unsigned char buffer[FRAME_NUMBER_OF_FRAME_BYTES];
    struct LRPFrame *next;
} LRPFrame;

// Statuses
#define FRAME_READY_TO_REDEFINE 0u

#define FRAME_BROADCAST_ID 0b11111u

#ifdef    __cplusplus
}
#endif

#endif //LOW_RESOURCE_PROTOCOL_FRAME_DATA_H
