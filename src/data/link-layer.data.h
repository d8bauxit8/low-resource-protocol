//
// Created by Jehoda Márk on 2023. 02. 03..
//

#ifndef LOW_RESOURCE_PROTOCOL_LINK_LAYER_DATA_H
#define LOW_RESOURCE_PROTOCOL_LINK_LAYER_DATA_H

#ifdef    __cplusplus
extern "C" {
#endif

#define LINK_LAYER_STATUS_OK 0u
#define LINK_LAYER_STATUS_SKIP 1u
#define LINK_LAYER_STATUS_ERROR 2u

typedef enum {
    OK = LINK_LAYER_STATUS_OK, Skip = LINK_LAYER_STATUS_SKIP, Error = LINK_LAYER_STATUS_ERROR
} LRPLinkLayerStatus;

#define LINK_LAYER_NO_ERROR 0u
#define LINK_LAYER_NOISE_STROKE_ERROR 1u
#define LINK_LAYER_DECODE_ERROR 2u
#define LINK_LAYER_INTERNAL_ERROR 3u

typedef enum {
    NoError = LINK_LAYER_NO_ERROR,
    NoiseStrokeError = LINK_LAYER_NOISE_STROKE_ERROR,
    DecodeError = LINK_LAYER_DECODE_ERROR,
    InternalError = LINK_LAYER_INTERNAL_ERROR
} LRPLinkLayerErrorCode;

#ifdef    __cplusplus
}
#endif

#endif //LOW_RESOURCE_PROTOCOL_LINK_LAYER_DATA_H
