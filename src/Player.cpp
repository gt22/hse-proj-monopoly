#include "Player.h"

PlayerData::PlayerData(std::string name, Token token) : name(std::move(name)), token(token) {}

Player::Player(Token token) : token(token) {}

bool PlayerData::checkDouble(bool newThrow) {
    doubleDice += newThrow ? 1 : 0;
    return (doubleDice >= 3);
}