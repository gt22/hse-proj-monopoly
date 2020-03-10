#ifndef GAME_H
#define GAME_H

#include "MonopolyFwd.h"
#include "Board.h"
#include "PlayerRequests.h"
#include <string_view>
#include <memory>
#include <vector>

class Game {
public:
    Game(const std::vector<std::pair<std::string_view, Token>>& players, Manager&);
    void runGame();
    PlayerReply sendRequest(Token token, PlayerRequest request);

private:
    //std::unique_ptr<View> view;
    Board board;
    Manager& manager;
};

#endif //GAME_H
