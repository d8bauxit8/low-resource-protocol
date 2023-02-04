/* 
 * File:   transmitter-application-layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LOW_RESOURCE_PROTOCOL_TRANSMITTER_APPLICATION_LAYER_H
#define LOW_RESOURCE_PROTOCOL_TRANSMITTER_APPLICATION_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../data/session-provider.data.h"

void LRP_TransmitterApplicationLayer_setDataIntoReservedFrame(LRPTransmitterSessionProvider *sessionProvider,
                                                              unsigned char *data,
                                                              unsigned char dataLength);

void LRP_TransmitterApplicationLayer_transmitReservedFrame(LRPTransmitterSessionProvider *sessionProvider,
                                                           unsigned char targetId,
                                                           unsigned char command);

unsigned char
LRP_TransmitterApplicationLayer_setReadyToRedefineFrameToReserved(LRPTransmitterSessionProvider *sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LOW_RESOURCE_PROTOCOL_TRANSMITTER_APPLICATION_LAYER_H */

