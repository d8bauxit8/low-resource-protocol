#include "link-layer.h"

void LRP_LinkLayer_setSkip(LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = Skip;
    sessionProvider->linkLayerErrorCode = NoError;
}

void LRP_LinkLayer_setError(LRPSessionProvider *const sessionProvider, LRPLinkLayerErrorCode errorCode) {
    sessionProvider->linkLayerStatus = Error;
    sessionProvider->linkLayerErrorCode = errorCode;
}

void LRP_LinkLayer_setOk(LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = OK;
    sessionProvider->linkLayerErrorCode = NoError;
}

unsigned char LRP_LinkLayer_isStatusOK(const LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == OK;
}

unsigned char LRP_LinkLayer_isStatusError(const LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == Error;
}

unsigned char LRP_LinkLayer_isError(const LRPSessionProvider *const sessionProvider, LRPLinkLayerErrorCode errorCode) {
    return sessionProvider->linkLayerErrorCode == errorCode;
}
