#ifndef __DEVICE_H_
#define __DEVICE_H_

#include "input.h"
#include "output.h"
#include "gpio.h"
#include "can_driver.h"

namespace device {
    void init(void* pxptr);
    void init_can_message(driver::can::message_t* message);
}


class Device {
    private:
        Output outputs[TOTAL_GPIO];
        driver::can::message_t can_mes;

    public:
        Device();
        uint8_t id {0};
        void init();
        void heartbeat();
        void toggle_switch(uint8_t switch_id);
        void double_press_switch(uint8_t switch_id);
        void hold_switch(uint8_t switch_id);
        void release_switch(uint8_t switch_id);
        void on_can_message(driver::can::message_t message);
        Input inputs[TOTAL_GPIO];
};

#endif