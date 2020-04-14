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

typedef struct {
    unsigned char sourceDeviceId;
    unsigned char targetDeviceId;
    unsigned char command;
    unsigned char status;
    unsigned char data[FRAME_DATA_LENGTH];
} _LRPFrame;

typedef struct {
    _LRPFrame *frameBuffer;
    unsigned char *transmitDeviceId;
} _LRPTransmitLayer;

typedef struct {
    _LRPFrame *frameBuffer;
    unsigned char numberOfReadBytes;
    _LRPFrame *currentFrame;
    unsigned char frameBufferLength;
    unsigned char status;
    unsigned char *receiveDeviceId;

    void (*framingErrorHandler)(void);

    void (*overrunErrorHandler)(void);

    void (*parityBitErrorHandler)(void);
} _LRPReceiveLayer;


#ifdef    __cplusplus
}
#endif

#endif    /* STRUCTURES_H */

