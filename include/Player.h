#ifndef PLAYER_H
#define PLAYER_H

#include <cstddef>
#include <string>

enum class Token : std::size_t {
    DOG,
    TYPE2,
    TYPE3
};

//TODO: возможно не надо
struct PlayerData {
    std::string name;
    std::size_t position;
    Token token;
    int daysLeftInPrison; //TODO: придумать нормальное имя
};

class Player {
public:
    PlayerData data;
    virtual ~Player();

};

class NetworkPlayer : public Player {

};

class BotPlayer : public Player {

};

class LocalPlayer : public Player {

};

#endif //PLAYER_H
