#ifndef MANAGER_H
#define MANAGER_H

#include "MonopolyFwd.h"
#include "Player.h"
#include "Game.h"
#include "network/MonopolyClient.h"
#include <vector>
#include <memory>
#include <thread>
#include <future>

class Manager {
public:
    ~Manager();
    void addPlayer(std::unique_ptr<Player> player);
    void run();
    void startGame();
    void startRemoteGame(const sockpp::inet_address& addr);
    bool isGameStarted();
    PlayerReply sendRequest(PlayerRequest request);
    void sync(const BoardModel& board);
private:

    void createView();
    void createGame();
private:

    std::thread gameThread;

    std::shared_ptr<SFMLView> view;
    std::shared_ptr<GameBase> game;
    std::vector<std::unique_ptr<Player>> players;
};

#endif //MANAGER_H
