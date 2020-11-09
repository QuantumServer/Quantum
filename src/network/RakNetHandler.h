#pragma once

#include <string>

#include "RakPeerInterface.h"

class RakNetHandler {

    public:
        void Initialize(int port, std::string ip, int max_players, std::string server_name);
        void Handle(RakNet::RakPeerInterface *peer);

};
