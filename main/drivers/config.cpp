#include "config_driver.h"

#include <string.h>

#include "nvs_flash.h"
#include "esp_log.h"

const static char* TAG = "config";
static nvs_handle_t my_handle;
static char string_buffer[160] {0};

void config::init(){
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition was truncated and needs to be erased");
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    ESP_LOGI(TAG, "Opening Non-Volatile Storage (NVS) handle... ");
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    ESP_LOGI(TAG, "init done");
}

uint8_t config::get_key(const char* key){
    int8_t value {0};
    nvs_get_i8(my_handle, key, &value);
    return static_cast<uint8_t>(value);
}

void config::set_key(const char* key, uint8_t value){
    nvs_set_i8(my_handle, key, value);
    nvs_commit(my_handle);
}

char* config::get_string(const char* key){
    size_t length = static_cast<size_t>(sizeof(string_buffer));
    return nvs_get_str(my_handle, key, string_buffer, &length) == ESP_OK ? string_buffer : NULL;
}

void config::set_string(const char* key, const char* value){
    nvs_set_str(my_handle, key, value);
    nvs_commit(my_handle);
}