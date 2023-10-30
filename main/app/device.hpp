#pragma once

#include "config.hpp"
#include "ginco_types.hpp"

using data::GincoMessage;

namespace app
{
    class Device
    {
    private:
        driver::ConfigDriver& config_;

    public:
        Device() : config_(driver::ConfigDriver::instance()){};
        void init();
        void handleMessage(GincoMessage &message);
        // void heartbeat();
        // void toggle_switch(uint8_t switch_id);
        // void double_press_switch(uint8_t switch_id);
        // void hold_switch(uint8_t switch_id);
        // void release_switch(uint8_t switch_id);
    };

} // namespace app;
