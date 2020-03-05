#ifndef FIELD_H
#define FIELD_H

#include "MonopolyFwd.h"
#include <string>

class FieldTile {
public:
    FieldTile(Board& board, int position, std::string name);
    virtual ~FieldTile() = default;
    Board& board;
    int position;
    std::string name;
    virtual void onPlayerPass(Token);
    virtual void onPlayerEntry(Token);
};

class OwnableTile final : public FieldTile {
public:
    int cost;
    int numberOfHouses;
    int costPerHouse;
    Token owner; //TODO: как обозначить отсутствие владельца

    void onPlayerEntry(Token) override;
};

class Start final : public FieldTile {
public:
    void onPlayerEntry(Token) override;
    void onPlayerPass(Token) override;
};

class Prison final : public FieldTile {
public:
    void onPlayerEntry(Token) override;

};

class Chance final : public FieldTile {
public:
    void onPlayerEntry(Token) override;
};


#endif //FIELD_H
