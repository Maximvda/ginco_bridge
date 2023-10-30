#pragma once

#include "standard_task.hpp"
#include "can_thread.hpp"
#include "ginco_thread.hpp"
#include "network_thread.hpp"

using utils::SupervisedTask;

namespace app {

	class TaskList {
	private:
        app::CanTask can_ {3};
		app::GincoTask ginco_ {1};
		app::NetworkTask network_ {2};

		const std::vector<SupervisedTask *> tasks_ {
			&can_,
			&ginco_,
			&network_
		};

		TaskList() = default;

	public:
		// only Supervisor can create the TaskList
		friend class Supervisor;

		TaskList(const TaskList&) = delete;
		auto& tasks() { return tasks_; }

		auto& can() { return can_;}
		auto& ginco() { return ginco_;}
		auto& network() { return network_;}
	};
}
