/* 
 * File:   structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef STRUCTURES_H
#define    STRUCTURES_H

#ifdef    __cplusplus
extern "C" {
#endif

#define FRAME_LENGTH 6
#define FRAME_DATA_LENGTH 4

typedef struct _FrameData {
    unsigned char sourceDeviceId;
    unsigned char targetDeviceId;
    unsigned char command;
    unsigned char data[FRAME_DATA_LENGTH];
} _FrameData;

typedef struct _LRPFrame {
    struct _FrameData;
    unsigned char status;
    struct _LRPFrame *next;
} _LRPFrame;

typedef void (*_LRPErrorHandler)(void);

typedef struct {
    _LRPFrame *frameBuffer;
    unsigned char numberOfReadBytes;
    unsigned char status;
    unsigned char *receiveDeviceId;

    _LRPFrame *handlerCurrentFrame;
    _LRPFrame *controllerCurrentFrame;

    _LRPErrorHandler framingErrorHandler;
    _LRPErrorHandler overrunErrorHandler;
    _LRPErrorHandler parityBitErrorHandler;
} _LRPReceiveLayer;


#ifdef    __cplusplus
}
#endif

#endif    /* STRUCTURES_H */

