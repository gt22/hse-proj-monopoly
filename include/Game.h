#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include <array>

class FieldTile {
public:
    int position;
    std::string name;
    virtual void onPlayerPass(Token);
    virtual void onPlayerEntry(Token);
};

class OwnableTile : public FieldTile {
public:
    int cost;
    int numberOfHouses;
    int costPerHouse;
    Token owner;//TODO: как обозначить отсутствие владельца

    void onPlayerEntry(Token) override;
};

class Prison : public FieldTile {
public:
    void onPlayerEntry(Token) override;
};

class Start : public FieldTile {
public:
    void onPlayerPass(Token) override;
};

class Chance : public FieldTile {
public:
    void onPlayerPass(Token) override;
};

class Game {
public:
    static constexpr int FIELD_SIZE = 40;
    std::array <FieldTile *, FIELD_SIZE>  field;

    void runGame();
};

#endif //GAME_H
