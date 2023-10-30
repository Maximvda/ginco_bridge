#include "network_thread.hpp"

using app::NetworkTask;
using utils::Message;

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
	default:
		assert(0);
	}
}
