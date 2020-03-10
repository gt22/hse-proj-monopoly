#include "Board.h"

PlayerData::PlayerData(std::string_view name, Token token) : name(name), token(token) {}

Board::Board(const std::vector<std::pair<std::string_view, Token> > & playersList, Game& game) : field(), deck(), game(game) {
    players.reserve(playersList.size());
    for (auto [name, token] : playersList) {
        players.emplace_back(name, token);
    }
    //TODO: Fill field with tiles
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