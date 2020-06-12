#include "link_layer.h"

void LRP_LinkLayer_setSkip(_LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LINK_LAYER_STATUS_SKIP;
}

void LRP_LinkLayer_setOk(_LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LINK_LAYER_STATUS_OK;
}

unsigned char LRP_LinkLayer_isStatusOK(_LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == LINK_LAYER_STATUS_OK;
}