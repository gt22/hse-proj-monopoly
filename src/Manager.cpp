#include "Manager.h"

PlayerReply Manager::sendRequest(Token token, PlayerRequest request) {
    for (std::size_t i = 0; i < players.size(); i++) {
        if (players[i]->token == token) {
            return players[i]->sendRequest(request);
        }
    }
    throw 1;
}