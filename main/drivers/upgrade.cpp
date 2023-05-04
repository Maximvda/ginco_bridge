#include "upgrade.h"

#include "varint_decode.h"
#include "esp_log.h"

using namespace upgrade;

const char * TAG = "Upgrade";
static Handler handler;
static Receiver receiver;

Handler::Handler(void){
	ESP_LOGI(TAG, "So my remote upgrade works? my handler");
};

void Handler::init(const esp_mqtt_event_t& event){
	uint32_t protoBufLength;
	uint8_t * ptr = (uint8_t *) event.data;
	uint32_t processed;

	ptr = smp_varint_decode(ptr, &protoBufLength);
	processed = smp_varint_encoding_length(protoBufLength) + protoBufLength;
	Ginco__Command* command = ginco__command__unpack(NULL, protoBufLength, ptr);
	ptr += protoBufLength;
	if (!command)
		return;

	if(!receiver.init(command)){
		receiver.fail();
	} else {
		receiver.receive(ptr, event.data_len - processed);
	}
    ginco__command__free_unpacked(command, NULL);
};

void Handler::handle(const esp_mqtt_event_t & event) {
	receiver.receive(reinterpret_cast<uint8_t *> (event.data), event.data_len);
};

void Handler::end(){
	receiver.complete();
};

Receiver::Receiver(void){};


bool Receiver::init(Ginco__Command * command) {
	otaPartition = esp_ota_get_next_update_partition(NULL);
	if (otaPartition == NULL){
		ESP_LOGE(TAG, "Passive OTA partition not found");
		return false;
	}
	if (command->upgrade->imagesize > otaPartition->size){
		ESP_LOGE(TAG, "Image size too large");
		return false;
	}

	ESP_ERROR_CHECK(esp_ota_begin(otaPartition, command->upgrade->imagesize, &update_handle));
	ESP_LOGI(TAG, "esp_ota_begin succeeded");
	return true;
};

bool Receiver::partitionValid(void){
	esp_app_desc_t newDescription;
	const esp_app_desc_t * currentDescription = esp_app_get_description();
	esp_err_t err = esp_ota_get_partition_description(otaPartition, &newDescription);
	if (err != ESP_OK) {
		ESP_LOGW(TAG, "ota get description returned %d ", err);
		return false;
	}
	if (strcmp(currentDescription->project_name, newDescription.project_name)) {
		ESP_LOGW(TAG, "Invalid image name %s, should be %s", newDescription.project_name, currentDescription->project_name);
		return false;
	}
	return true;
};

bool Receiver::receive(const uint8_t * data, uint32_t len) {
	ESP_ERROR_CHECK(esp_ota_write(update_handle, data, len));
	return true;
};

void Receiver::complete(){
	ESP_ERROR_CHECK(esp_ota_end(update_handle));
	if (!partitionValid())
		return;

	ESP_ERROR_CHECK(esp_ota_set_boot_partition(otaPartition));
	ESP_LOGI(TAG, "esp_ota_set_boot_partition succeeded");
	esp_restart();
};

void Receiver::fail(){
	if(update_handle)
		esp_ota_end(update_handle);
};