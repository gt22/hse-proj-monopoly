#ifndef MANAGER_H
#define MANAGER_H

#include "MonopolyFwd.h"
#include "Player.h"
#include "Game.h"
#include "View.h"

#include <vector>
#include <memory>
#include <variant>

class Manager {
public:
    Manager();
    void addPlayer(std::unique_ptr<Player> player);
    PlayerReply sendRequest(Token token, PlayerRequest request);
    void startNewGame();
private:

    std::shared_ptr<ViewHolder> view;
    std::shared_ptr<Game> game;
    std::vector<std::unique_ptr<Player>> players;
};

#endif //MANAGER_H
