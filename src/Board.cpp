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

