#ifndef GAME_H
#define GAME_H

#include "MonopolyFwd.h"
#include "Board.h"
#include "PlayerRequests.h"
#include <string_view>
#include <memory>
#include <vector>
#include "Random.h"

class Game {
public:
    Game(const std::vector<std::pair<std::string_view, Token>>& players, Manager&);
    void runGame();
    PlayerReply sendRequest(Token token, PlayerRequest request);
    NumReply sendNumRequest(Token token);
    SumReply sendSumRequest(Token token);
    PlayerTradeReply sendTradeRequest(Token token, PlayerTradeRequest request);
    void sendMessage(Token token, PlayerMessage mes, MessageType type);
    Board& getBoard();
    void sync();

private:
    //std::unique_ptr<View> view;
    RandomSource rng;
    Board board;
    Manager& manager;
};

#endif //GAME_H
