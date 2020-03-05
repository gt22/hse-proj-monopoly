#ifndef PLAYER_H
#define PLAYER_H

#include "MonopolyFwd.h"
#include <string>
#include <vector>

enum class Token : std::size_t {
    DOG,
    TYPE2,
    TYPE3
};

struct PlayerRequest {
    std::string message;
};

struct PlayerReply {
    bool answer;
};

class Player {
public:
    Player(Token token);
    virtual ~Player() = default;
    virtual PlayerReply sendRequest(PlayerRequest request) = 0;
    virtual void sync(const Board& playerData) = 0;

    const Token token;
};

class NetworkPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const Board& playerData) override;
};

class BotPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const Board& playerData) override;
};

class LocalPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const Board& playerData) override;

private:
    //что-то, чтобы обращаться к view
};

#endif //PLAYER_H
