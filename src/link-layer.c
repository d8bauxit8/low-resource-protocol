#include "link-layer.h"

void LRP_LinkLayer_setSkip(LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LRPLinkLayerStatus_Skip;
    sessionProvider->linkLayerErrorCode = LRPLinkLayerErrorCode_NoError;
}

void LRP_LinkLayer_setError(LRPSessionProvider *const sessionProvider, LRPLinkLayerErrorCode errorCode) {
    sessionProvider->linkLayerStatus = LRPLinkLayerStatus_Error;
    sessionProvider->linkLayerErrorCode = errorCode;
}

void LRP_LinkLayer_setOk(LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LRPLinkLayerStatus_OK;
    sessionProvider->linkLayerErrorCode = LRPLinkLayerErrorCode_NoError;
}

unsigned char LRP_LinkLayer_isStatusOK(const LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == LRPLinkLayerStatus_OK;
}

unsigned char LRP_LinkLayer_isStatusError(const LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == LRPLinkLayerStatus_Error;
}

unsigned char LRP_LinkLayer_isError(const LRPSessionProvider *const sessionProvider, LRPLinkLayerErrorCode errorCode) {
    return sessionProvider->linkLayerErrorCode == errorCode;
}
