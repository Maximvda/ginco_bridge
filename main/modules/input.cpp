#include "input.h"

#include <string>

#include "config_driver.h"
#include "esp_log.h"
#include "device.h"
#include "can_driver.h"
#include "gpio.h"

#define PRESS_DELAY 0.4*1000*1000
//#define PERIODIC_DELAY 0.01*1000*1000
#define PERIODIC_DELAY 0.4*1000*1000

const static char* TAG = {"input"};

using namespace driver;

Input::Input(){};

Input::Input(uint8_t new_id, void (*_callback)(void* arg), void (*_hold_callback)(void* arg)){
    id = new_id;
    char config_key[9] = {"inputx"};
    sprintf(config_key, "input%i", new_id);
    if (config::get_key(config_key) == 1){
        button = true;
        ESP_LOGI(TAG, "Input %i is configured as button", id);
    }
    // Creating the timers
    const esp_timer_create_args_t timer_args = {
        .callback = _callback,
        .arg = &this->id
    };
    esp_timer_create(&timer_args, &press_timer);
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = _hold_callback,
        .arg = &this->id
    };
    esp_timer_create(&periodic_timer_args, &hold_timer);
    // Creating default can message
    can_mes.event = true;
    can_mes.source_id = id;
    can_mes.ack = false;
}

void Input::handle_message(can::message_t rec_can_mes){
    switch(rec_can_mes.function_address){
        case 0xFF: // Request state (doing in end of call)
            break;
        default:
            break;
    }
    // Always aknowledge and return state
    rec_can_mes.ack = true;
    uint8_t value = static_cast<uint8_t>(gpio::get_level(id));
    can::transmit(rec_can_mes);
}

void Input::press_callback(){
    switch (current_press){
        case 2:
            can::transmit(can_mes);
            ESP_LOGI(TAG, "First press");
            break;
        case 4:
            ESP_LOGI(TAG, "Second press");
            break;
        case 6:
            ESP_LOGI(TAG, "Thirs press");
        default:
            break;
    }
    current_press = 0; // Reset the press count
}

void Input::hold_callback(){
    ESP_LOGI(TAG, "Hold callback %u", this->id);
}


void Input::toggle(){
    current_press++;
    if(!button){
        press_callback();
        return;
    }
    // Timer already active so we are handling second or third press
    if (esp_timer_is_active(press_timer) and current_press % 2 == 0 ){
        esp_timer_stop(press_timer);
    }

    // Start timers when button switch
    esp_timer_start_once(press_timer, PRESS_DELAY);
    // Only start hold timer when current press is 0 (first press)
    if (current_press == 1 and !hold_active){
        hold_active = true;
        esp_timer_start_periodic(hold_timer, PERIODIC_DELAY);
    }
    // Hold timer active so we held button and stop now -> stop timer
    else if (esp_timer_is_active(hold_timer)){
        esp_timer_stop(hold_timer);
        hold_active = false;
    }
}

void Input::set_button(bool value){
    char config_key[9] = {"inputx"};
    sprintf(config_key, "input%i", id);
    config::set_key(config_key, value ? 1 : 0);
}