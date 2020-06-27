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

void LRP_TransmitApplicationLayer_setDataIntoReservedFrame(_LRPSessionProvider *const sessionProvider,
                                                           const unsigned char *const data,
                                                           const unsigned char const dataLength);

void LRP_TransmitApplicationLayer_transmitReservedFrame(_LRPSessionProvider *const sessionProvider,
                                                        const unsigned char const targetDeviceId,
                                                        const unsigned char const command);

unsigned char
LRP_TransmitApplicationLayer_setReadyToRedefineFrameToReserved(_LRPSessionProvider *const sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LRP_TRANSMIT_APPLICATION_LAYER_H */

