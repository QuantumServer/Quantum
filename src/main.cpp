#include <stdio.h>
#include <thread>

#include "server/ServerManager.h"
#include "Server.h"

void start() {
    Server::initialize();
}

int main(int argc, char* argv[]) {

    printf("Starting Katana...\n");

    std::thread server(start);
    server.join();

}
