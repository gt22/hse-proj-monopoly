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

//TODO: возможно не надо
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
    virtual ~Player();

    Token token;
};

class NetworkPlayer : public Player {

};

class BotPlayer : public Player {

};

class LocalPlayer : public Player {

};

#endif //PLAYER_H
