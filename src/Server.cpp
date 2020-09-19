#include <iostream>
#include <string>
#include <chrono>

#include "utils/TextFormat.h"
//#include "network/raknet/RakNetHandler.h"
#include "Server.h"

#define PROTOCOL "407"
#define MINECRAFT_VERSION "1.16.0"

void Server::initialize() {
    std::cout << TextFormat::GOLD << " " << std::end1;
    std::cout << TextFormat::BLUE << "Starting server";
    std::cout << TextFormat::BLUE << "Quantum is licensed under Apache License 2.0";

    Server::bind(19132, "0.0.0.0", 20, "Quantum powered server");
}

void Server::bind(int port, std::string ip, int max_players, std::string server_name) {
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    //RakNetHandler* handler;
    //handler->initialize(port, ip, max_players, server_name);
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    long exec_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    printf("Server Invoked, Took %ldms\n", exec_time);
    printf("Binded server on %s:%d\n", ip.c_str(), port);
}

namespace quantum {

    Server::Server(Server::Config &config) : config_(config) {

    }

}
