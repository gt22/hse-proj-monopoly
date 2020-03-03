#ifndef GAME_H
#define GAME_H

//#include "View.h"
#include "MonopolyFwd.h"
#include "Board.h"
#include "Player.h" 

#include <vector>

class Game {
public:
    Game(const std::vector<std::pair<std::string, Token> > & players) :
            board(std::vector<std::pair<std::string, Token> > & players);
    void runGame();

private:
    //std::unique_ptr<View> view;
    Board board;
};

#endif //GAME_H
