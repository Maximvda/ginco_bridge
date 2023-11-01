#include "network.hpp"

using app::NetworkManager;
using driver::ConfigKey;

void NetworkManager::init()
{
    controller_.setHandler(
        [this](NetworkEvent event){
            this->eventHandler(event);
        }
    );
    config_.setKey(ConfigKey::WIFI_CONFIGURED, true);
    if(config_.getKey<bool>(ConfigKey::WIFI_CONFIGURED))
    {
        controller_.init(driver::NetworkAdapter::STA);
    }
    else
    {
        controller_.init(driver::NetworkAdapter::AP);
    }
}

void NetworkManager::eventHandler(NetworkEvent event)
{
    switch(event)
    {
        case NetworkEvent::CONNECTED:
        {
            (void)mqtt_.init(config_.getKey<std::string>(ConfigKey::MQTT_URL));
            break;
        }
        default:
            break;
    }
}