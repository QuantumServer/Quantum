/*
 * ServerManager
 * Copyright (C) 2016 iTX Technologies
 *
 * This file is part of Cenisys.
 *
 * Cenisys is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Cenisys is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Cenisys.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "server/servermanager.h"
#include <fstream>
#include <boost/asio/signal_set.hpp>
#include <boost/assert.hpp>
#include <boost/fiber/asio/use_future.hpp>
#include <boost/fiber/operations.hpp>
#include <boost/thread/null_mutex.hpp>

namespace Cenisys {
	ServerManager::ServerManager(const std::string &configPath)
	sched_data_(service_) {
		BOOST_ASSERT(!instance_);
		instance_ = this;
		std::ifstream conf(configPath);
		toml::ParseResult result = toml::parse(conf);
		ConfigParser<MasterConfig>()(result.value, config_);
		std::size_t threads = config_.threads;
		if(threads == 0)
			threads = std::max(1U, std::thread::hardware_concurrency());
		for(std::size_t i = 1; i != threads; ++i) {
			threads_.emplace_back([&] {
				boost::fibers::use_scheduling_algorithm<boost::fibers::algo::asio>(
				sched_data_);
				boost::null_mutex lk;
				thread_join_.wait(lk);
			});
		}
		std::vector<boost::fibers::fiber> server_fibers;
		for(auto &i : config_.server) {
			server_fibers.emplace_back(
			[&] { servers_.emplace(i.first, i.second); });
		}
		for(auto &i : server_fibers) {
			i.join();
		}
	}
	ServerManager::~ServerManager() {
		std::vector<boost::fibers::fiber> server_fibers;
		for(auto &i : config_.server) {
			server_fibers.emplace_back([&] { servers_.erase(i.first); });
		}
		for(auto &i : server_fibers) {
			i.join();
		}
		thread_join_.notify_all();
		for(auto &i : threads_) {
			i.join();
		}
		BOOST_ASSERT(instance_);
		instance_ = nullptr;
	}
	
	int ServerManager::exec() {
		boost::asio::signal_set kill(service_, SIGTERM);
		auto term_future = kill.async_wait(boost::fibers::asio::use_future);
		term_future.wait();
		return 0;
	}
	
	boost::asio::io_service &ServerManager::getIoService() {
		return service_;
	}
	
	ServerManager *ServerManager::getInstance() {
		return instance_;
	}
	ServerManager *ServerManager::instance_{};
} // namespace Cenisys