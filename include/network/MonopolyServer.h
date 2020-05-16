#pragma once
#include <sockpp/tcp_acceptor.h>
#include <unordered_map>
#include <thread>
#include "MonopolyFwd.h"
#include "include/threads/BoardModel.h"
#include "threads/ModelThreadManager.h"
namespace Monopoly::Network {

    using ClientId = std::size_t;

    class ClientHandler {
    public:
        void sync(const Board& board);
        PlayerReply sendRequest(Player& p, PlayerRequest request);
        void sendMessage(Player& p, PlayerMessage message);
    private:

        void mainLoop();

        ClientId id;
        std::thread th;
        sockpp::tcp_socket socket;
        Threads::ModelThreadManager model;
    };

    class ConnectionAcceptor {
    public:

        ~ConnectionAcceptor();

    private:
        sockpp::tcp_acceptor acceptor;
        std::unordered_map<ClientId, ClientHandler> clinets;
    };

}