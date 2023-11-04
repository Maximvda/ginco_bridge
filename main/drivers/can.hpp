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
    public:
        void init();
        void tick();

        /* When blocking is true we'll wait for an acknowledge to be received! */
        bool transmit(GincoMessage &message, bool blocking = false);
    };

}