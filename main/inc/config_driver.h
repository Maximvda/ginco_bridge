#ifndef __CONFIG_DRIVER_H_
#define __CONFIG_DRIVER_H_

#include "stdint.h"
#include "nvs_flash.h"

#define CONFIG_KEY_MQTT "MQTT_URL"
#define CONFIG_KEY_CONFIGURED "CONFIGURED"

namespace config {
    void init();
    uint8_t get_key(const char* key);
    void set_key(const char* key, uint8_t value);
    char* get_string(const char* key);
    void set_string(const char* key, const char* value);
}

#endif