#pragma once

#include "standard_task.hpp"

#include "can.hpp"
#include "concurrent.hpp"
#include "events.h"

using driver::can::CanDriver;
using utils::Milliseconds;
using utils::StandardTask;

namespace app
{

    class CanTask : public StandardTask
    {

    private:

        void handle(utils::Message &) override;
        void onStart() override;
        void tick() override;
        Milliseconds queueTimeout() override { return 10; };

        void handleCanMes(const GincoMessage& mes);

    public:
        CanDriver can_driver;
        bool upgrading {false};

        CanTask(uint32_t priority) : StandardTask(priority){};
        const char *name() const override { return "Can"; }

        bool transmit(GincoMessage &message)
        {
            return post(EVENT_CAN_TRANSMIT, std::make_unique<GincoMessage>(message));
        }

    };

}
