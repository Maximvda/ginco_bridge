#pragma once

#include "esp_wifi.h"
#include "lwip/sockets.h"

namespace driver
{
    class SocketListener
    {
    private:
        static constexpr uint8_t TOTAL_RECORS {16};
        wifi_ap_record_t records_[TOTAL_RECORS];

        struct sockaddr_in dest_addr_
        {
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_family = AF_INET;
            .sin_port = htons(30675)
        };

        int compareRssi(const void* first, const void* second);

    public:
        bool init(void);
    };
} //namespace driver