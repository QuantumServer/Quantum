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

namespace quantum {
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
}

#endif
