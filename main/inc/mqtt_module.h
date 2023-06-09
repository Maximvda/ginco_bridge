#ifndef _MY_MQTT_HEADER_H_
#define _MY_MQTT_HEADER_H_

#include "mqtt_client.h"

#define MQTT_KEY "MQTT_URL"
#define WIFI_KEY "SSID"
#define WIFI_PASS "WIFI_PASSWORD"

namespace mqtt {
    bool init();
    void send_message(const char *data);
    bool connected(void);
    bool set_url(char* url);
}

#endif