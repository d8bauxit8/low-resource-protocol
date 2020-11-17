#include "link_layer.h"

void LRP_LinkLayer_setSkip(_LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    sessionProvider->linkLayerErrorCode = LINK_LAYER_NO_ERROR;
}

void LRP_LinkLayer_setError(_LRPSessionProvider *const sessionProvider, const unsigned char errorCode) {
    sessionProvider->linkLayerStatus = LINK_LAYER_STATUS_ERROR;
    sessionProvider->linkLayerErrorCode = errorCode;
}

void LRP_LinkLayer_setOk(_LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LINK_LAYER_STATUS_OK;
    sessionProvider->linkLayerErrorCode = LINK_LAYER_NO_ERROR;
}

unsigned char LRP_LinkLayer_isStatusOK(_LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == LINK_LAYER_STATUS_OK;
}

unsigned char LRP_LinkLayer_isStatusError(_LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == LINK_LAYER_STATUS_ERROR;
}

unsigned char LRP_LinkLayer_isError(_LRPSessionProvider *const sessionProvider, const unsigned char errorCode) {
    return sessionProvider->linkLayerErrorCode == errorCode;
}
