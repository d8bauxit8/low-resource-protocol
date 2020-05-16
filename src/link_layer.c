#include "link_layer.h"

void LRP_LinkLayer_setError(_LRPSessionProvider *const sessionProvider) {
    sessionProvider->linkLayerStatus = LINK_LAYER_STATUS_SKIP;
    LRP_Frame_resetStatus(sessionProvider->linkCurrentFrame);
}

unsigned char LRP_LinkLayer_isStatusOK(_LRPSessionProvider *const sessionProvider) {
    return sessionProvider->linkLayerStatus == LINK_LAYER_STATUS_OK;
}