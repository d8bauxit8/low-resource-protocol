//
// Created by Jehoda Márk on 2023. 02. 03..
//

#ifndef LOW_RESOURCE_PROTOCOL_LINK_LAYER_DATA_H
#define LOW_RESOURCE_PROTOCOL_LINK_LAYER_DATA_H

#ifdef    __cplusplus
extern "C" {
#endif

#define LRP_LINK_LAYER_STATUS_OK 0u
#define LRP_LINK_LAYER_STATUS_SKIP 1u
#define LRP_LINK_LAYER_STATUS_ERROR 2u

typedef enum {
    LRPLinkLayerStatus_OK = LRP_LINK_LAYER_STATUS_OK,
    LRPLinkLayerStatus_Skip = LRP_LINK_LAYER_STATUS_SKIP,
    LRPLinkLayerStatus_Error = LRP_LINK_LAYER_STATUS_ERROR
} LRPLinkLayerStatus;

#define LRP_LINK_LAYER_NO_ERROR 0u
#define LRP_LINK_LAYER_NOISE_STROKE_ERROR 1u
#define LRP_LINK_LAYER_DECODE_ERROR 2u
#define LRP_LINK_LAYER_INTERNAL_ERROR 3u

typedef enum {
    LRPLinkLayerErrorCode_NoError = LRP_LINK_LAYER_NO_ERROR,
    LRPLinkLayerErrorCode_NoiseStrokeError = LRP_LINK_LAYER_NOISE_STROKE_ERROR,
    LRPLinkLayerErrorCode_DecodeError = LRP_LINK_LAYER_DECODE_ERROR,
    LRPLinkLayerErrorCode_InternalError = LRP_LINK_LAYER_INTERNAL_ERROR
} LRPLinkLayerErrorCode;

#ifdef    __cplusplus
}
#endif

#endif //LOW_RESOURCE_PROTOCOL_LINK_LAYER_DATA_H
