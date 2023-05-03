#include "connection_manager.h"

#include <stdio.h>
#include <atomic>

#include "esp_wifi_types.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"

#include "config_driver.h"
#include "ip_interface.h"

using namespace ip_interface;


static WifiApIpInterface wifiApIpInterface;
static WifiStaIpInterface wifiStaIpInterface;
static IpInterface * const ipInterfaces[] = {&wifiApIpInterface, &wifiStaIpInterface};

class CConnectionManagerBase;

static std::atomic<CConnectionManagerBase*> DriverInstance;

static eAction_t driverCallback(SignaList signal, adapter_t iface)
{
    eAction_t result = eAction_t::eActionNone;
    if (DriverInstance == nullptr)
        return result;
    switch(signal){
        case SIGNAL_IPDRIVER_DOWN:
            DriverInstance.OnDriverDown();
    }
    return result;
}

void connection_manager::init(){
    ip_interface::init()
}

bool connection_manager::start(eConnectionType type){
    switch(type){

    }
}