#include "upgrade.hpp"

#include "esp_log.h"

#include "varint_decode.h"
#include "ginco.pb-c.h"
#include "ginco_types.hpp"

#include "supervisor.hpp"

static const char *TAG = "Upgrader";

using component::UpgradeHandler;
using component::Upgrader;
using data::GincoMessage;
using data::ConfigFunction;


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
    private:
        GincoMessage message_;
        uint32_t mes_counter_ {0};
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
    app::taskFinder().can().upgrading = true;
    message_.source(command.device_id);
    message_.function<ConfigFunction>(ConfigFunction::UPGRADE);
    message_.data<uint32_t>(command.image_size);
    ESP_LOGI(TAG, "image size %lu    %lu", command.image_size, message_.data<uint32_t>());
    message_.send(true);
    ESP_LOGI(TAG, "Start acknowledged");
    message_.function<ConfigFunction>(ConfigFunction::FW_IMAGE);
    return true;
};

bool CanUpgrader::receive(const uint8_t *data, uint32_t len)
{
    /* After this we'll transfer the FW image*/
    while (len > 8)
    {
        mes_counter_++;
        if ((mes_counter_ % 1000) == 0)
        {
            ESP_LOGI(TAG, "sent %lu", mes_counter_);
        }
        len -= 8;
        message_.data<const uint8_t*>(data, 8);
        data += 8;
        message_.send(true);
    }

    message_.data<const uint8_t*>(data, len);
    return message_.send(true);
};

void CanUpgrader::complete()
{
    app::taskFinder().can().upgrading = false;
    message_.function<ConfigFunction>(ConfigFunction::UPGRADE_FINISHED);
    message_.send(); /* Don't aknowledge this message */
    ESP_LOGI(TAG, "finished");
};

void CanUpgrader::fail()
{
    app::taskFinder().can().upgrading = false;
    /* Init can't fail so no upgrade fail command */
};