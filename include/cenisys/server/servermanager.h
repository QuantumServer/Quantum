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
#ifndef CENISYS_SERVERMANAGER_H
#define CENISYS_SERVERMANAGER_H

#include <thread>
#include <unordered_map>
#include <vector>
#include <boost/asio/io_service.hpp>
#include <boost/fiber/algo/asio.hpp>
#include <boost/fiber/condition_variable.hpp>
#include "config/configparser.h"
#include "server.h"

using ServerList = std::unordered_map<std::string, Cenisys::Server::Config>;
CENISYS_DEFINE_CONFIG_PARSER((Cenisys), MasterConfig, (int, threads, 0), (ServerList, server))

namespace Cenisys {
	class ServerManager {
		public:
			struct Config;
			ServerManager(const std::string &configPath);
			~ServerManager();
			
			int exec();
			
			boost::asio::io_service &getIoService();
			
			static ServerManager *getInstance();
			
		private:
			static ServerManager *instance_;
			boost::asio::io_service service_;
			boost::fibers::algo::asio::shared_data sched_data_;
			boost::fibers::condition_variable_any thread_join_;
			std::vector<std::thread> threads_;
			std::unordered_map<std::string, Server> servers_;
			MasterConfig config_;
	};
} // namespace Cenisys

#endif // CENISYS_SERVERMANAGER_H