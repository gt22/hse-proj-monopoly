#ifndef MANAGER_H
#define MANAGER_H

#include "MonopolyFwd.h"
#include "Player.h"
#include "Game.h"
#include "View.h"
#include "Menu.h"
#include <vector>
#include <memory>
#include <thread>
#include <future>

class Manager {
public:
    void addPlayer(std::unique_ptr<Player> player);
    void run();
    void startGame();
    bool isGameStarted();
    const Board& getBoard();
    PlayerReply sendRequest(Token token, PlayerRequest request);
    void sendMessage(Token token, PlayerMessage request);
    void sync(const Board& board);
private:

    void createView();
    void createGame();

    std::thread gameThread;

    std::shared_ptr<SFMLView> view;
    std::shared_ptr<Game> game;
    std::vector<std::unique_ptr<Player>> players;
};

#endif //MANAGER_H
