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

class OwnableTile : public FieldTile {
public:
    int cost;
    int costOfParking;
    Token owner; //TODO: как обозначить отсутствие владельца

    void onPlayerEntry(Token token) override;
};

class Railway final : OwnableTile {

    void onPlayerEntry(Token token) override;
};

class Street final : OwnableTile {
    int numberOfHouses;
    int costPerHouse;

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

class IncomeTax final : public FieldTile {
public:
    void onPlayerEntry(Token token) override;
};


#endif //FIELD_H
