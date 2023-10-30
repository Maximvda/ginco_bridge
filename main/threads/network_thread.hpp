#pragma once

#include "standard_task.hpp"
#include "concurrent.hpp"
#include "events.h"
#include "network.hpp"

using utils::StandardTask;
using utils::Milliseconds;

namespace app
{

	class NetworkTask : public StandardTask
	{

	private:
		void handle(utils::Message &) override;
		void onStart() override;
		void onTimeout() override;
		Milliseconds queueTimeout() override { return 10; };

		NetworkManager network_man_;

	public:
		NetworkTask(uint32_t priority) : StandardTask(priority) {}

		const char *name() const override { return "Network"; }
	};

}
