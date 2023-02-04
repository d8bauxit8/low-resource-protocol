#include "link-layer.h"

void LRP_LinkLayer_setSkip(LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LRP_LINK_LAYER_STATUS_SKIP;
    sessionProvider->linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
}

void LRP_LinkLayer_setError(LRPSessionProvider *const sessionProvider, const unsigned char errorCode) {
    sessionProvider->linkLayerStatus = LRP_LINK_LAYER_STATUS_ERROR;
    sessionProvider->linkLayerErrorCode = errorCode;
}

void LRP_LinkLayer_setOk(LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LRP_LINK_LAYER_STATUS_OK;
    sessionProvider->linkLayerErrorCode = LRP_LINK_LAYER_NO_ERROR;
}

unsigned char LRP_LinkLayer_isStatusOK(const LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == LRP_LINK_LAYER_STATUS_OK;
}

unsigned char LRP_LinkLayer_isStatusError(const LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == LRP_LINK_LAYER_STATUS_ERROR;
}

unsigned char LRP_LinkLayer_isError(const LRPSessionProvider *const sessionProvider, unsigned char errorCode) {
    return sessionProvider->linkLayerErrorCode == errorCode;
}
