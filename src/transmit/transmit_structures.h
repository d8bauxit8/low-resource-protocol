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
#define TRANSMIT_FRAME_RESERVED 0b00000001
#define TRANSMIT_FRAME_READY_TO_CHECK 0b00000010
#define TRANSMIT_FRAME_READY_TO_TRANSMIT 0b00000011
#define TRANSMIT_FRAME_TRANSMITTING 0b00000100

#ifdef    __cplusplus
}
#endif

#endif    /* TRANSMIT_STRUCTURES_H */


