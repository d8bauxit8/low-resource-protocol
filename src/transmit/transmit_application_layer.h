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

void LRP_TransmitApplicationLayer_setDataIntoReservedFrame(_LRPSessionProvider *sessionProvider,
                                                           const unsigned char *data,
                                                           unsigned char dataLength);

void LRP_TransmitApplicationLayer_transmitReservedFrame(_LRPSessionProvider *sessionProvider,
                                                        unsigned char targetDeviceId,
                                                        unsigned char command);

unsigned char
LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(_LRPSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_TRANSMIT_APPLICATION_LAYER_H */

