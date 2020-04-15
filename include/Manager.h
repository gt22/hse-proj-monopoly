#ifndef MANAGER_H
#define MANAGER_H

#include "MonopolyFwd.h"
#include "Player.h"
#include "Game.h"
#include "View.h"
#include "Menu.h"
#include <vector>
#include <memory>


class Manager {
public:
    void addPlayer(std::unique_ptr<Player> player);
    void createMenu();
    void startGame();
    PlayerReply sendRequest(Token token, PlayerRequest request);
    void sendMessage(Token token, PlayerMessage request);
    std::shared_ptr<ViewHolder> view;
    const Board& getBoard();
    void sync(const Board& board);
private:
    void createGame();
    std::unique_ptr<Game> game;
    std::vector<std::unique_ptr<Player>> players;
};

#endif //MANAGER_H
