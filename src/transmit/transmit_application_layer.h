/* 
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LRP_TRANSMIT_APPLICATION_LAYER_H
#define    LRP_TRANSMIT_APPLICATION_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "transmit_structures.h"

void LRP_TransmitApplicationLayer_setDataIntoReservedFrame(LRPTransmitSessionProvider *sessionProvider,
                                                           unsigned char *data,
                                                           unsigned char dataLength);

void LRP_TransmitApplicationLayer_transmitReservedFrame(LRPTransmitSessionProvider *sessionProvider,
                                                        unsigned char targetDeviceId,
                                                        unsigned char command);

unsigned char
LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(LRPTransmitSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_TRANSMIT_APPLICATION_LAYER_H */

