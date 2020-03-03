#ifndef PLAYER_H
#define PLAYER_H

#include "MonopolyFwd.h"
#include <cstddef>
#include <string>

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

struct PlayerData {
    PlayerData(std::string name, Token token);
    std::string name;
    Token token;
    std::size_t position = 0;
    int money = 0;
    int doubleDice = 0;
    int daysLeftInPrison = 0; //TODO: придумать нормальное имя
    bool cardToLeftPrison = false;
    bool skip = false;

    bool checkDouble(bool newThrow);
};

class Player {
public:
    Player(Token token);
    virtual ~Player() = default;
    virtual PlayerReply sendRequest(PlayerRequest request) = 0;
    virtual void sync(const PlayerData& playerData) = 0;

    const Token token;
};


class NetworkPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const PlayerData& playerData) override;
};

class BotPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const PlayerData& playerData) override;
};

class LocalPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const PlayerData& playerData) override;
};

#endif //PLAYER_H
