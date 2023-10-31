#include "network.hpp"

using app::NetworkManager;
using driver::ConfigKey;

void NetworkManager::init()
{
    if(config_.getKey<bool>(ConfigKey::WIFI_CONFIGURED))
    {
        controller_.init(driver::NetworkAdapter::STA);
    }
    else
    {
        controller_.init(driver::NetworkAdapter::AP);
    }
    (void)mqtt_.init(config_.getKey<std::string>(ConfigKey::MQTT_URL));
}