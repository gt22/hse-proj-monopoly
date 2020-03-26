#include "Manager.h"
#include "Game.h"
#include <string_view>
#include <cassert>

void Manager::addPlayer(std::unique_ptr<Player> player) {
    players.push_back(std::move(player));
}

PlayerReply Manager::sendRequest(Token token, PlayerRequest request) {
    for(auto& p : players) {
        if(p->token == token) {
            return p->sendRequest(std::move(request));
        }
    }
    throw 1; //TODO: сделать PlayerNotFoundException или что-нибудь такое
}

void Manager::sendMessage(Token token, PlayerMessage mes) {
    for(auto& p : players) {
        if(p->token == token) {
            p->sendMessage(std::move(mes));
            return;
        }
    }
    throw 1; //TODO: сделать PlayerNotFoundException или что-нибудь такое
}

void Manager::createGame() {
    assert(!game);
    std::vector<std::pair<std::string_view, Token>> gameData;
    gameData.reserve(players.size());
    for(const auto& p : players) {
        gameData.emplace_back(std::string_view (p->name), p->token);
    }
    game = std::make_unique<Game>(gameData, *this);
}

