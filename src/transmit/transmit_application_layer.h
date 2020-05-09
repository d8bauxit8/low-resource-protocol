/* 
 * File:   layers.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef TRANSMIT_APPLICATION_LAYER_H
#define    TRANSMIT_APPLICATION_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "transmit_structures.h"

void LRP_setDataIntoReservedFrame(_LRPSessionProvider *const sessionProvider, const unsigned char *const data,
                                  const unsigned char const dataLength);

void LRP_transmitReservedFrame(_LRPSessionProvider *const sessionProvider,
                               const unsigned char const targetDeviceId, const unsigned char const command);

unsigned char LRP_setReadyToRedefineFrameToReserved(_LRPSessionProvider *const sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* TRANSMIT_APPLICATION_LAYER_H */

