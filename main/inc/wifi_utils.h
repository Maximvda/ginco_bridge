#ifndef _WIFI_UTILS_H_
#define _WIFI_UTILS_H_

#include "esp_wifi.h"


namespace wifi_utils {
    esp_err_t setMode(wifi_mode_t mode);
    esp_err_t getConfig(wifi_interface_t iface, wifi_config_t &wifi_config);
    esp_err_t setConfig(wifi_interface_t iface , wifi_config_t &wifi_config  );
    esp_err_t startWifi();
    esp_err_t startWifiScan(const void * scan_config);
    esp_err_t stopWifi();
    esp_err_t connect();
}

#endif