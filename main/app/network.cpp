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
}