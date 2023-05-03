#include "http_client.h"

#include "esp_log.h"
#include "esp_http_client.h"
#include "config_driver.h"
#include "connection_manager.h"

using namespace http_client;

#define MAX_HTTP_OUTPUT_BUFFER 2048

static const char *TAG = "Con manager";

static void http_start(void *pvParameters);

void http_client::init(){
    //TODO:: Receive the config and swap interfaces
    config::set_string(CONFIG_KEY_SSID, "TODO");
    config::set_string(CONFIG_KEY_PASS, "TODO");
    connection_manager::start(ip_interface::eAdapterType::ADAPTER_STA);
    //xTaskCreate(&http_start, "http_test_task", 8192, NULL, 5, NULL);
}

void http_start(void *pvParameters){
    vTaskDelay(1000);
    char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};   // Buffer to store response of http request
    int content_length = 0;
    esp_http_client_config_t config = {
        .url = "http://192.168.4.2/",
        .port = 8080
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET Request
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    } else {
        content_length = esp_http_client_fetch_headers(client);
        if (content_length < 0) {
            ESP_LOGE(TAG, "HTTP client fetch headers failed");
        } else {
            int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
            if (data_read >= 0) {
                ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRIu64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
                ESP_LOG_BUFFER_HEX(TAG, output_buffer, data_read);
            } else {
                ESP_LOGE(TAG, "Failed to read response");
            }
        }
    }
    esp_http_client_close(client);
    vTaskDelete(NULL);
}