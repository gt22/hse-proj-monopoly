#include <Board.h>

Board::Board(const std::vector<std::pair<std::string, Token> > & playersList) {
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
}

int Board::getNumberOfPlayers() {
    return (int)players.size();
}