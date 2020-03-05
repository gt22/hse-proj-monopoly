#ifndef GAME_H
#define GAME_H

#include "MonopolyFwd.h"
#include "Board.h"
#include <memory>
#include <vector>

class Game {
public:
    Game(const std::vector<std::pair<std::string, Token>>& players, std::shared_ptr<Manager> manager);
    void runGame();
    PlayerReply sendRequest(Token token, PlayerRequest request);

private:
    //std::unique_ptr<View> view;
    Board board;
    std::shared_ptr<Manager> manager;
};

#endif //GAME_H
