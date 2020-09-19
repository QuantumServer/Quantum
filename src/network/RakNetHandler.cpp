#include <string>
#include <sstream>

#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "BitStream.h"

#include "network/CppRakLibHandler.h"

#define PROTOCOL "390"
#define MINECRAFT_VERSION "1.14.60"

void CppRakLibHandler::initialize(int port, std::string ip, int max_players, std::string server_name) {
    CppRakLib::RakPeerInterface *peer = CppRakLib::RakPeerInterface::getInstance();
    CppRakLib::SocketDescriptor socket_desc(port, ip.c_str());

    if (peer->Startup(max_players, &socket_desc, 1) == CppRakLib::CPPRAKLIB_STARTED) {
        peer->SetMaximumIncomingConnections(max_players);
        std::ostringstream motd;
        motd << "MCPE;" << server_name << ";" << PROTOCOL << ";" << MINECRAFT_VERSION << ";0;" << max_players;
        std::string message = motd.str();

        message.insert(message.begin(), message.size());
        message.insert(message.begin(), 0x00);

        peer->SetOfflinePingResponse(message.c_str(), message.size());

        while (1) {
            CppRakLibHandler::handle(peer);
        }
    }
}

void CppRakLibHandler::handle(CppRakLib::RakPeerInterface *peer) {
    CppRakLib::packet *packet;
    for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
        switch (packet->data[0]) {
            printf("[%s]: %s", packet->data[0], packet->data);
            case 0xFE:
                CppRakLib::BitStream stream(packet->data, packet->length, false);
                stream.IgnoreBytes(sizeof(CppRakLib::MessageID));

                int length;
                stream.Read(length);
                break;
        }
    }
}
