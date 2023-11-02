#include "upgrade.hpp"

#include "esp_log.h"

#include "varint_decode.h"
#include "ginco.pb-c.h"

static const char *TAG = "Upgrader";

using component::UpgradeHandler;
using component::Upgrader;

/* Forward delcaration */
class SelfUpgrader : public Upgrader
{
    private:
        const esp_partition_t* ota_partition_;
        esp_ota_handle_t update_handle_;

        bool partitionValid();
    public:
        bool init(Ginco__Upgrade& command);
        bool receive(const uint8_t * data, uint32_t len);
        void complete();
        void fail();
};
class CanUpgrader : public Upgrader {
    public:
        bool init(Ginco__Upgrade& command);
        bool receive(const uint8_t * data, uint32_t len);
        void complete();
        void fail();
};

UpgradeHandler::UpgradeHandler(const esp_mqtt_event_t& event)
{
    uint32_t proto_length;
    uint8_t *ptr = (uint8_t *)event.data;
    uint32_t processed;

    ptr = smp_varint_decode(ptr, &proto_length);
    processed = smp_varint_encoding_length(proto_length) + proto_length;
    Ginco__Command *command = ginco__command__unpack(NULL, proto_length, ptr);
    ptr += proto_length;
    if (!command)
        return;

    /* Upgrade was for gateway */
    if (command->upgrade->device_id == 0)
    {
        active_receiver = std::make_unique<SelfUpgrader>();
    }
    else
    {
        active_receiver = std::make_unique<CanUpgrader>();
    }

    if (!active_receiver->init(*command->upgrade))
    {
        active_receiver->fail();
    }
    else
    {
        active_receiver->receive(ptr, event.data_len - processed);
    }
    ginco__command__free_unpacked(command, NULL);
};

void UpgradeHandler::handle(const esp_mqtt_event_t& event)
{
    active_receiver->receive(reinterpret_cast<uint8_t *>(event.data), event.data_len);
};

void UpgradeHandler::complete()
{
    active_receiver->complete();
};

bool SelfUpgrader::init(Ginco__Upgrade& upgrade_cmd)
{
    ota_partition_ = esp_ota_get_next_update_partition(NULL);
    if (ota_partition_ == NULL)
    {
        ESP_LOGE(TAG, "Passive OTA partition not found");
        return false;
    }
    if (upgrade_cmd.image_size > ota_partition_->size)
    {
        ESP_LOGE(TAG, "Image size too large");
        return false;
    }

    ESP_ERROR_CHECK(esp_ota_begin(ota_partition_, upgrade_cmd.image_size, &update_handle_));
    ESP_LOGI(TAG, "esp_ota_begin succeeded");
    return true;
};

bool SelfUpgrader::partitionValid()
{
    esp_app_desc_t new_description;
    const esp_app_desc_t * current_description = esp_app_get_description();
    esp_err_t err = esp_ota_get_partition_description(ota_partition_, &new_description);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "ota get description returned %d ", err);
        return false;
    }
    if (strcmp(current_description->project_name, new_description.project_name)) {
        ESP_LOGW(TAG, "Invalid image name %s, should be %s", new_description.project_name, current_description->project_name);
        return false;
    }
    return true;
};

bool SelfUpgrader::receive(const uint8_t *data, uint32_t len)
{
    ESP_ERROR_CHECK(esp_ota_write(update_handle_, data, len));
    return true;
};

void SelfUpgrader::complete()
{
    ESP_ERROR_CHECK(esp_ota_end(update_handle_));
    if (!partitionValid())
        return;

    ESP_ERROR_CHECK(esp_ota_set_boot_partition(ota_partition_));
    ESP_LOGI(TAG, "esp_ota_set_boot_partition succeeded");
    esp_restart();
};

void SelfUpgrader::fail()
{
    if (update_handle_)
        esp_ota_end(update_handle_);
};

bool CanUpgrader::init(Ginco__Upgrade& command)
{
    /* TODO: Send can message*/
    return true;
};

bool CanUpgrader::receive(const uint8_t *data, uint32_t len)
{
//     uint64_t buffer;
//     uint8_t retries = {0};
//     while (len > 8)
//     {
//         len -= 8;
//         buffer = 0;
//         for (int i = 0; i < 8; i++)
//         {
//             buffer += (*data << 8 * i);
//             ++data;
//         }
//         can_message.data = buffer;
//         can_message.buffer_size = 8;
//         while (!driver::can::transmit(can_message, true))
//         {
//             retries += 1;
//             ESP_LOGI(TAG, "Retry for upgrade %u", retries);
//         }
//     }
//     buffer = 0;
//     for (int i = 0; i < 8; i++)
//     {
//         buffer += (*data << 8 * i);
//         ++data;
//     }
//     can_message.data = buffer;
//     can_message.buffer_size = 8;
//     while (!driver::can::transmit(can_message, true))
//     {
//         retries += 1;
//         ESP_LOGI(TAG, "Retry for upgrade %u", retries);
//     }

    return true;
};

void CanUpgrader::complete()
{
    // fail();
};

void CanUpgrader::fail()
{
    // Transmit ending message
    // driver::can::transmit(can_message, true);
};