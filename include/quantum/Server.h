#ifndef CENISYS_SERVER_H
#define CENISYS_SERVER_H

#include <utility>
#include "config/configparser.h"

class Server {

    public:
        struct Config;
        Server(Config &config);

        static void initialize();
        static void bind(int port, std::string ip_addr, int max_players, std::string server_name);

        void onServerStart();
        void onServerStop();

    private:
        std::reference_wrapper<Config> config_;

};

CENISYS_DEFINE_CONFIG_PARSER((Cenisys), Server::Config, (int, max_players, 64))

#endif
