#ifndef BOARD_H
#define BOARD_H

#include "MonopolyFwd.h"
#include "Player.h"
#include "Cards.h"
#include <array>
#include <vector>

class Board {
public:
    Board(const std::vector<std::pair<std::string, Token> > & players, Game& game);
    static constexpr int FIELD_SIZE = 40;
    std::array <FieldTile *, FIELD_SIZE>  field;
    CardPool deck;

    PlayerData& getPlayer(Token);
    int getNumberOfPlayers();
    PlayerReply sendRequest(Token token, PlayerRequest request);

private:
    std::vector<PlayerData> players;
    Game& game;
};

#endif //BOARD_H
