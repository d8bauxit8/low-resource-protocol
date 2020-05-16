/* 
 * File:   transmit_structures.h
 * Author: D8bauxit8
 *
 * Created on April 6, 2020, 4:30 PM
 */

#ifndef TRANSMIT_STRUCTURES_H
#define    TRANSMIT_STRUCTURES_H

#ifdef    __cplusplus
extern "C" {
#endif

#include "../session_provider.h"

// Status flow
// 1. Ready to redefine
// 2. Reserved
// 3. Ready to check
// 3. Ready to transmit
// 4. Transmitting
#define TRANSMIT_FRAME_RESERVED 1
#define TRANSMIT_FRAME_READY_TO_CHECK 2
#define TRANSMIT_FRAME_READY_TO_TRANSMIT 3
#define TRANSMIT_FRAME_TRANSMITTING 4

typedef struct {
    struct _LRPSessionProvider;
    unsigned char indexOfWroteBytes;
} _LRPTransmitSessionProvider;

#ifdef    __cplusplus
}
#endif

#endif    /* TRANSMIT_STRUCTURES_H */


