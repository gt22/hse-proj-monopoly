#include "Board.h"

PlayerData::PlayerData(std::string name, Token token) : name(std::move(name)), token(token) {}

Board::Board(const std::vector<std::pair<std::string, Token> > & playersList, Game& game) : game(game) {
    players.reserve(playersList.size());
    /*for (auto &[name, token] : playersList) {
        players.emplace_back(name, token);
    }*/
    for (size_t i = 0; i < playersList.size(); i++) {
        std::string name = playersList[i].first;
        Token token = playersList[i].second;
        players.emplace_back(name, token);
    }

}

PlayerData& Board::getPlayer(Token token) {
    for (auto &player : players) {
        if (player.token == token) {
            return player;
        }
    }
    //TODO: что вернуть если не нашли.
    throw 1;
}

std::size_t Board::getPlayersNumber() {
    return players.size();
}

Token Board::getPlayerToken(std::size_t index) {
    return players[index].token;
}

void PlayerData::newPosition(std::size_t throwSum) {
    std::size_t newPoz = (position + throwSum) % 40;
    if (newPoz < position) {
        money += 200;
    }
    position = (position + throwSum) % 40;
}

void PlayerData::toPrison() {
    position = PRISON_POZ;
    daysLeftInPrison = 3;
    doubleDice = 0;
    prisoner = true;
}

PlayerReply Board::sendRequest(Token token, PlayerRequest request) {
    return game.sendRequest(token, request);
}