#ifndef FIELD_H
#define FIELD_H

#include "MonopolyFwd.h"
#include <string>

static constexpr int PRISON_FINE = 50;

class FieldTile {
public:
    FieldTile(Board& board, int position, std::string name);
    virtual ~FieldTile() = default;
    Board& board;
    int position;
    std::string name;
    virtual void onPlayerPass(Token token);
    virtual void onPlayerEntry(Token token);
};

class OwnableTile final : public FieldTile {
public:
    int cost;
    int numberOfHouses;
    int costPerHouse;
    Token owner; //TODO: как обозначить отсутствие владельца

    void onPlayerEntry(Token token) override;
};

class Start final : public FieldTile {
public:
    void onPlayerPass(Token token) override;
};

class Prison final : public FieldTile {
public:
    void onPlayerEntry(Token token) override;
};

class Chance final : public FieldTile {
public:
    void onPlayerEntry(Token token) override;
};


#endif //FIELD_H
