#pragma once

/* std includes */
#include <map>

/* ginco includes */
#include "config.hpp"
#include "ginco_types.hpp"
#include "switch_co.hpp"

using data::GincoMessage;
using modules::SwitchCo;

namespace app
{
    class Device
    {
    private:
        driver::ConfigDriver& config_;
        std::map<uint8_t, SwitchCo> switches_;

    public:
        Device() : config_(driver::ConfigDriver::instance()){};
        void init();
        /* Handle config messages */
        void handleConfig(GincoMessage &message);
    };

} // namespace app;
