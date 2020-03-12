#ifndef MANAGER_H
#define MANAGER_H

#include "MonopolyFwd.h"
#include "Player.h"
#include "Game.h"
#include <vector>
#include <memory>


class Manager {
public:
    void addPlayer(std::unique_ptr<Player> player);
    PlayerReply sendRequest(Token token, PlayerRequest request);

private:
    void createGame();

    //std::unique_ptr<MenuView> menu;
    std::unique_ptr<Game> game;
    std::vector<std::unique_ptr<Player>> players;
};

#endif //MANAGER_H
