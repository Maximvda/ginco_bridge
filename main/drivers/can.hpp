#pragma once

/* std includes */
#include <functional>

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
        using MessageCb = std::function<void(const GincoMessage& mes)>;
        MessageCb message_cb_;
    public:
        void init(MessageCb cb_fnc);
        void tick();

        /* When blocking is true we'll wait for an acknowledge to be received! */
        bool transmit(GincoMessage &message, bool blocking = false);
    };

}