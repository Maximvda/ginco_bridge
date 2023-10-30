#pragma once

/* Esp includes */
#include "driver/twai.h"

/* Ginco includes */
#include "ginco_types.hpp"

using data::GincoMessage;

namespace driver::can
{

    class CanDriver
    {
    private:
        GincoMessage last_message_;
    public:
        void init();
        void tick();

        bool transmit(const GincoMessage &message);
    };

}