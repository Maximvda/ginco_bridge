#pragma once

#include "esp_wifi.h"

#include <functional>
#include <map>

namespace driver
{
    enum class NetworkAdapter
    {
        AP,
        STA,
    };

    enum class NetworkEvent
    {
        CONNECTED,
    };

    /* Forward declaration to use it in handle event as context */
    class NetworkController;

    class IpInterface
    {
    private:
        virtual void init() = 0;
        virtual void start();
        virtual void handleEvent(NetworkController& context, int32_t id, void* data) = 0;
        friend class NetworkController;
        virtual const char * name() const = 0;
        virtual wifi_mode_t mode() const = 0;
        virtual esp_netif_t* interface() const = 0;
    };

    class WifiStaI : private IpInterface
    {
    private:
        bool config_pending_ {true};
        wifi_config_t wifi_config_;
        NetworkAdapter adapter_{NetworkAdapter::STA};
        esp_netif_t* interface_;
        virtual void init();
        virtual void handleEvent(NetworkController& context, int32_t id, void* data);
        friend class NetworkController;
        const char *name() const override { return "sta"; }
        wifi_mode_t mode() const override { return WIFI_MODE_STA; }
        esp_netif_t* interface() const override { return interface_; }
    };

    class WifiApI : private IpInterface
    {
    private:
        wifi_config_t wifi_config_;
        NetworkAdapter adapter_{NetworkAdapter::AP};
        esp_netif_t* interface_;
        virtual void init();
        virtual void handleEvent(NetworkController& context, int32_t id, void* data);
        friend class NetworkController;
        const char *name() const override { return "ap"; }
        wifi_mode_t mode() const override { return WIFI_MODE_AP; }
        esp_netif_t* interface() const override { return interface_; }
    };

    class NetworkController
    {
    private:
        WifiStaI sta_i_;
        WifiApI ap_i_;
        // std::map<NetworkAdapter, IpInterface*> i_map_ {{NetworkAdapter::AP, &ap_i_}, {NetworkAdapter::STA, &sta_i_}};
        IpInterface *const interfaces_[2] {&ap_i_, &sta_i_};
        IpInterface* active_interface_;

    public:
        std::function<void(NetworkEvent)> event_handler {nullptr};

        void init(const NetworkAdapter& adapter);
        void handleEvent(esp_event_base_t event, int32_t id, void* data);

        void start(const NetworkAdapter& adapter);

        void setSsid(const char* ssid, const char* pass);

        void setHandler(std::function<void(NetworkEvent)> cb_fnc);
    };
} // namespace driver