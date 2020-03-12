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

