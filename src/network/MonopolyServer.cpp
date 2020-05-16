#include "network/MonopolyServer.h"

#include <utility>

namespace Monopoly::Network {


    void ClientHandler::sync(const Board& board) {
        model.sync(board);
    }

    PlayerReply ClientHandler::sendRequest(Player& p, PlayerRequest request) {
        return model.processRequest(p, std::move(request));
    }

    void ClientHandler::sendMessage(Player& p, PlayerMessage message) {
        model.processMessage(p, std::move(message));
    }

    void ClientHandler::mainLoop() {
        stream
    }


}