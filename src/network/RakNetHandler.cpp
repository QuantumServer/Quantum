#include <string>
#include <sstream>

#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "BitStream.h"

#include "network/protocol/Info.h"
#include "network/RakNetHandler.h"

void RakNetHandler::initialize(int port, std::string ip, int max_players, std::string server_name) {
    RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::getInstance();
    RakNet::SocketDescriptor socket_desc(port, ip.c_str());

    if (peer->Startup(max_players, &socket_desc, 1) == RakNet::CPPRAKLIB_STARTED) {
        peer->SetMaximumIncomingConnections(max_players);

        std::ostringstream motd;
        motd << "MCPE;" << server_name << ";" << Info::CURRENT_PROTOCOL << ";" << Info::MINECRAFT_VERSION << ";0;" << max_players;
        std::string message = motd.str();

        message.insert(message.begin(), message.size());
        message.insert(message.begin(), 0x00);

        peer->SetOfflinePingResponse(message.c_str(), message.size());

        while (1) {
            RakNetHandler::handle(peer);
        }
    }
}

void RakNetHandler::handle(RakNet::RakPeerInterface *peer) {
    RakNet::packet *packet;
    for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
        switch (packet->data[0]) {
            printf("[%s]: %s", packet->data[0], packet->data);
            case 0xFE:
                RakNet::BitStream stream(packet->data, packet->length, false);
                stream.IgnoreBytes(sizeof(RakNet::MessageID));

                int length;
                stream.Read(length);
                break;
        }
    }
}
