#ifndef __CONNECTION_MANAGER_H_
#define __CONNECTION_MANAGER_H_

#include "stdint.h"

namespace connection_manager {
    enum class eConnectionType {
        WiFi_ap,
        WiFi_sta,
    };

    void init();
    bool start(eConnectionType type);
}

#endif