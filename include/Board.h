#ifndef GAME_H
#define GAME_H

#include "MonopolyFwd.h"
#include "Player.h"
#include <array>
#include <vector>

class Board {
public:
    Board(const std::vector<std::pair<std::string, Token> > & players);
    static constexpr int FIELD_SIZE = 40;
    std::array <FieldTile *, FIELD_SIZE>  field;
    //int numberOfPlayers;

    PlayerData& getPlayer(Token);
    int getNumberOfPlayers();

private:
    std::vector<PlayerData> players;
};

#endif //GAME_H
