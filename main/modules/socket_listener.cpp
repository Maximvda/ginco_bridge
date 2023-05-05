#include "socket_listener.h"

#include <cstring>
#include <memory>

#include "esp_log.h"
#include "lwip/sockets.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "ip_interface.h"
#include "config_driver.h"
#include "connection_manager.h"

using namespace socket_listener;

#define PORT 30675

typedef enum {
    CONFIGURE
} commands_t;

static const char *TAG = "socket listener";
static wifi_ap_record_t records[25];

static void config_listener_task(void *pvParameters);

esp_err_t socket_listener::init(){
	BaseType_t result = xTaskCreate(config_listener_task, TAG, 8192, NULL, 5, NULL);
	return (result == pdPASS) ? ESP_OK : ESP_FAIL;
}

static std::unique_ptr<char[]> readItem(int sock) {
    unsigned char length;

    int len = recv(sock, &length, 1, 0);
    if (len != 1) {
        ESP_LOGE(TAG, "receive length byte failed");
        return nullptr;
    }
    auto result = std::unique_ptr<char[]>(new char[length + 1]);
    len = recv(sock, result.get(), length, 0);
    if (len != length) {
        ESP_LOGE(TAG, "receive string failed");
        return nullptr;
    } else {
        result[length] = 0;
        return result;
    }
}

static bool configure(int sock) {

    std::unique_ptr<char[]> password = nullptr;
    std::unique_ptr<char[]> mqtt = nullptr;
    auto ssid = readItem(sock);
    if (ssid) {
        password = readItem(sock);
    }
    if (password) {
        mqtt = readItem(sock);
    }
    uint8_t length = mqtt ? 1 : 0;
    send(sock, &length, 1, 0);
    if (mqtt) {
        bool gotssid = strlen(ssid.get()) > 0;
        if (gotssid) {
            ip_interface::setSsidPass(ssid.get(), password.get());
        }
        config::set_string(CONFIG_KEY_MQTT, mqtt.get());
        config::set_key(CONFIG_KEY_CONFIGURED, 1);
        ESP_LOGI(TAG, "Config SSID: %s", ssid.get());
        ESP_LOGI(TAG, "Config pass: %s", password.get());
        ESP_LOGI(TAG, "Config mqtt: %s", mqtt.get());
    }
    return mqtt ? true : false;
}

static bool process(int sock) {
    unsigned char command;

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    int len = recv(sock, &command, 1, 0);
    if (len != 1) {
        ESP_LOGE(TAG, "receive command byte failed");
        return false;
    }
#if ENABLE_LOGGING
    ESP_LOGI(TAG, "processing command: %d",command );
#endif
    switch (command) {
        case CONFIGURE:
            return configure(sock);
        default:
            ESP_LOGE(TAG, "received invalid command byte");
            return false;
    }
}

static bool config_socketserver(int listen_sock)
{
    ESP_LOGI(TAG, "Socket listening");
    bool stop = false;
    do {
        int sock = accept(listen_sock, NULL, NULL);
        if (sock < 0) {
            stop = true;
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
        }
        else {
            ESP_LOGI(TAG, "Socket accepted");
            stop = process(sock);
            shutdown(sock, 0);
            close(sock);
        }
    } while (!stop);
    return stop;
}

static int doCompareRssi(wifi_ap_record_t* first, wifi_ap_record_t* second) {
    return (first->rssi == second->rssi) ? 0 : (first->rssi > second->rssi ? -1 : 1);
}
static int compareRssi(const void* first, const void* second) {
    return doCompareRssi((wifi_ap_record_t*)first, (wifi_ap_record_t*)second);
}

static void getSsids() {
    uint16_t ap_count {25};
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_count, records));
    qsort(records, 25, sizeof(wifi_ap_record_t), compareRssi);
}

static void terminate(int listen_sock) {
    if (listen_sock >= 0) {
        close(listen_sock);
    }
    vTaskDelay(pdMS_TO_TICKS(500));
    connection_manager::start(ip_interface::eAdapterType::ADAPTER_STA);
    vTaskDelete(NULL);
    return;
}

static void config_listener_task(void* pvParameters) {

    getSsids();

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);

    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        terminate(listen_sock);
        return;
    }
    int reuse = 1;
    int err = lwip_setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during setsockopt so_reuseadr: errno %d", errno);
        terminate(listen_sock);
        return;
    }
    ESP_LOGI(TAG, "Socket created");
    err = bind(listen_sock, (struct sockaddr*) & dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        terminate(listen_sock);
        return;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);
    err = listen(listen_sock, 1);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        terminate(listen_sock);
        return;
    }
    bool success = false;
    while (! success)
    {
        success = config_socketserver(listen_sock);
    }
    terminate(listen_sock);
}