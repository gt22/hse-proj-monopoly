#pragma once

#include <unordered_map>
#include <thread>
#include <sockpp/tcp_socket.h>
#include "include/threads/BoardModel.h"
#include "threads/ModelThreadManager.h"

namespace Monopoly::Network {

    class Client {
    public:
        Client(const sockpp::inet_address& addr, sockpp::tcp_socket socket,
               Threads::ModelThreadManager& model, std::function<void()> onDeath);

        void mainLoop();

    private:
        std::thread th;
        std::mutex socketMutex;
        sockpp::inet_address addr;
        sockpp::tcp_socket socket;
        Threads::ModelThreadManager& model;
        std::function<void()> onDeath;
    };

}