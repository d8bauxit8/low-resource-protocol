/* 
 * File:   transmit-application-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMIT_APPLICATION_LAYER_H
#define    LOW_RESOURCE_PROTOCOL_TRANSMIT_APPLICATION_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "transmit-structures.h"

void LRP_TransmitApplicationLayer_setDataIntoReservedFrame(LRPTransmitSessionProvider *sessionProvider,
                                                           unsigned char *data,
                                                           unsigned char dataLength);

void LRP_TransmitApplicationLayer_transmitReservedFrame(LRPTransmitSessionProvider *sessionProvider,
                                                        unsigned char targetId,
                                                        unsigned char command);

unsigned char
LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(LRPTransmitSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_TRANSMIT_APPLICATION_LAYER_H */

