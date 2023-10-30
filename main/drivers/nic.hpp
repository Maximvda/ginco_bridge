#pragma once

#include "esp_wifi.h"

namespace driver
{
    enum class NetworkAdapter
    {
        STA,
        AP
    };

    class IpInterface
    {
    private:
        NetworkAdapter adapter_;
        virtual void init(wifi_config_t &config) = 0;
        virtual void start();
        virtual void handleEvent(int32_t id, void* data) = 0;
        friend class NetworkController;
        virtual const char * name() const = 0;
    };

    class WifiStaI : private IpInterface
    {
    private:
        bool config_pending_ {false};
        NetworkAdapter adapter_{NetworkAdapter::STA};
        virtual void init(wifi_config_t &config);
        virtual void handleEvent(int32_t id, void* data);
        friend class NetworkController;
        const char *name() const override { return "sta"; }
    };

    class WifiApI : private IpInterface
    {
    private:
        NetworkAdapter adapter_{NetworkAdapter::AP};
        virtual void init(wifi_config_t &config);
        virtual void handleEvent(int32_t id, void* data);
        friend class NetworkController;
        const char *name() const override { return "ap"; }
    };

    class NetworkController
    {
    private:
        wifi_config_t wifi_config_;
        WifiStaI sta_i_;
        WifiApI ap_i_;
        IpInterface *const interfaces_[2] {&sta_i_, &ap_i_};
        IpInterface& active_interface_ {*interfaces_[0]};

    public:
        void init(NetworkAdapter adapter);
        void handleEvent(esp_event_base_t event, int32_t id, void* data);

        void setSsid(const char* ssid, const char* pass);
    };
} // namespace driver