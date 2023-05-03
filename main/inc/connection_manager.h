#ifndef __CONNECTION_MANAGER_H_
#define __CONNECTION_MANAGER_H_

#include "stdint.h"
#include "ip_interface.h"

namespace connection_manager {
    enum class eConnectionType {
        WiFi_ap,
        WiFi_sta,
    };

    void init();
    void start(ip_interface::eAdapterType type);
}

#endif