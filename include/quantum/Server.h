#ifndef CENISYS_SERVER_H
#define CENISYS_SERVER_H

#include <utility>
#include "config/configparser.h"

namespace quantum {
	class Server {

        public:
            struct Config;
            Server(Config &config);

        private:
            std::reference_wrapper<Config> config_;

    };

}

CENISYS_DEFINE_CONFIG_PARSER((Cenisys), Server::Config, (int, max_players, 64))

#endif
