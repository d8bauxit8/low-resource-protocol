/*
 * File:   link_layer.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 8:06 PM
 */

#ifndef LINK_LAYER_H
#define    LINK_LAYER_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "session_provider_structures.h"

#define LINK_LAYER_STATUS_OK 0
#define LINK_LAYER_STATUS_SKIP 1

void LRP_LinkLayer_setSkip(_LRPSessionProvider *const sessionProvider);

void LRP_LinkLayer_setOk(_LRPSessionProvider *const sessionProvider);

unsigned char LRP_LinkLayer_isStatusOK(_LRPSessionProvider *const sessionProvider);

#ifdef    __cplusplus
}
#endif

#endif    /* LINK_LAYER_H */

