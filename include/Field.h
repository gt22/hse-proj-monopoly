#ifndef FIELD_H
#define FIELD_H

#include "MonopolyFwd.h"
#include <string>

static constexpr int PRISON_FINE = 50;
static constexpr int START_SUM = 200;

enum class Color : std::size_t {
    COL1,
    COL2,
    COL3,
    COL4,
    COL5,
    COL6,
    COL7,
    COL8
};

enum class TileType : std::size_t {
    START = 0,
    OWNABLE_TILE = 1,
    PRISON = 2,
    GOTO_PRISON = 3,
    CHANCE = 4,
    INCOME_TAX = 5,
    FREE_PARKING = 6,
    UNINITIALIZED = 1000
};

enum class OwnableType : std::size_t {
    RAILWAY = 0,
    STREET = 1,
    UTILITY = 2,
    UNINITIALIZED = 1000
};

class FieldTile {
public:
    explicit FieldTile(Board& board);
    FieldTile(Board& board, TileType type, int position, std::string name);
    virtual ~FieldTile() = default;
    Board& board;
    TileType type = TileType::UNINITIALIZED;
    uint32_t position = 0;
    std::string name;
    virtual void onPlayerPass(Token token);
    virtual void onPlayerEntry(Token token);
};

class OwnableTile : public FieldTile {
public:
    explicit OwnableTile(Board& board);
    OwnableTile(Board& board, int position, std::string name,
                OwnableType ownableType, uint32_t cost, Color color);
    virtual uint32_t calculateTax(Token token) = 0;
    virtual void onPurchase(Token token) = 0;
    void onPlayerEntry(Token token) override;
    OwnableType ownableType = OwnableType::UNINITIALIZED;
    uint32_t cost = 0;
    uint32_t costOfParking = 0;
    Color color = Color::COL1;
    Token owner = Token::FREE_FIELD;
};

class Railway final : public OwnableTile {
public:
    explicit Railway(Board& board);
    Railway(Board& board, int position, std::string name,
            int cost, Color color);
    uint32_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
};

class Street final : public OwnableTile {
public:
    explicit Street(Board& board);
    Street(Board& board, int position, std::string name,
           int cost, Color color, int costPerHouse);
    uint32_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
    void onPlayerEntry(Token token) override;
    uint32_t numberOfHouses = 0;
    uint32_t costPerHouse = 0;
};

class Utility final : public OwnableTile {
public:
    explicit Utility(Board& board);
    Utility(Board& board, int position, std::string name,
            int cost, Color color);
    uint32_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
};

class Start final : public FieldTile {
public:
    explicit Start(Board& board);
    Start(Board& board, int position, std::string name);
    void onPlayerPass(Token token) override;
    void onPlayerEntry(Token token) override;
};

class Prison final : public FieldTile {
public:
    explicit Prison(Board& board);
    Prison(Board& board, int position, std::string name);
    void onPlayerEntry(Token token) override;
    const static int TAX = 50;
};

//Considered harmful
class GoToPrison final : public FieldTile {
public:
    explicit GoToPrison(Board& board);
    GoToPrison(Board& board, int position, std::string name);
    void onPlayerEntry(Token token) override;
};

class Chance final : public FieldTile {
public:
    explicit Chance(Board& board);
    Chance(Board& board, int position, std::string name);
    void onPlayerEntry(Token token) override;
};

class IncomeTax final : public FieldTile {
public:
    explicit IncomeTax(Board& board);
    IncomeTax(Board& board, int position, std::string name,
            uint32_t tax);
    void onPlayerEntry(Token token) override;
    uint32_t tax = 0;
};

class FreeParking final : public FieldTile {
public:
    explicit FreeParking(Board& board);
    FreeParking(Board& board, int position, std::string name);
};

#endif //FIELD_H
