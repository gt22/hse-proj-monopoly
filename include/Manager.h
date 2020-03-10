#ifndef MANAGER_H
#define MANAGER_H

#include "MonopolyFwd.h"
#include "Player.h"
#include <vector>
#include <memory>


class Manager {
public:
    //Manager(unique_ptr<MenuView> menu) : menu(menu) {
    // menu.setAddPlayerCallback([this](std::unique_ptr<Player> player) { this->addPlayer(player) })
    // };
    ~Manager(); //Default, needed to destroy `game` when `Game` is complete type
    void addPlayer(std::unique_ptr<Player> player);
    PlayerReply sendRequest(Token token, PlayerRequest request);

private:
    void createGame();

    //std::unique_ptr<MenuView> menu;
    std::unique_ptr<Game> game;
    std::vector<std::unique_ptr<Player>> players;
};

#endif //MANAGER_H
