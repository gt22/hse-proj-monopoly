#include "Player.h"
#include "Cards.h"

PlayerData::PlayerData(std::string name, Token token) : name(std::move(name)), token(token) {}

Player::Player(Token token) : token(token) {}
