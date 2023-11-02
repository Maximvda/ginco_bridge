#include "network_thread.hpp"

#include "nic.hpp"

using app::NetworkTask;
using utils::Message;
using driver::NetworkAdapter;

void NetworkTask::onStart()
{
	network_man_.init();
}

void NetworkTask::onTimeout()
{
}

void NetworkTask::handle(Message &message)
{
	switch (message.event())
	{
	case EVENT_START_STA_INTERFACE:
	{
		network_man_.startInterface(NetworkAdapter::STA);
		break;
	}
	case EVENT_SET_SSID_PASS:
	{
		if (auto m = message.takeValue<std::tuple<std::string, std::string>>())
		{
			auto& d = *m.get();
			network_man_.controller.setSsid(std::get<0>(d).data(), std::get<1>(d).data());
		}
		break;
	}
	default:
		assert(0);
	}
}
