#ifndef FIELD_H
#define FIELD_H

#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include <string>
#include <vector>

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

class FieldTile {
public:
    FieldTile(Board& board, int position, std::string name);
    virtual ~FieldTile() = default;
    Board& board;
    int position;
    std::string name;
    bool isMortgaged = false;
    virtual void onPlayerPass(Token token);
    virtual void onPlayerEntry(Token token);
    virtual std::vector<std::string> writeTileInfo();
    virtual void setMortgageCost(int val = 0) = 0;
    virtual bool MortgageTile(Token token) = 0;
    virtual Token getOwner() const {return Token::FREE_FIELD; }
    virtual int getMortgageCost() const { return 0; }
};

class OwnableTile : public FieldTile {
public:
    OwnableTile(Board& board, int position, std::string name,
            int cost, Color color);
    virtual size_t calculateTax(Token token) = 0;
    virtual void onPurchase(Token token) = 0;
    void onPlayerEntry(Token token) override;
    void setMortgageCost(int val = 0) override { mortgageCost = val; }
    bool MortgageTile(Token token) override;
    virtual std::vector<std::string> writeTileInfo() override;
    Token getOwner() const override {return owner; }
    int getMortgageCost() const override { return mortgageCost; }
    int mortgageCost = 0;
    int cost;
    int costOfParking = 0;
    Color color;
    Token owner = Token::FREE_FIELD; //TODO: как обозначить отсутствие владельца
};

class Railway final : public OwnableTile {
public:
    Railway(Board& board, int position, std::string name,
            int cost, Color color);
    size_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
    std::vector<std::string> writeTileInfo() override;
};

class Street final : public OwnableTile {
public:
    Street(Board& board, int position, std::string name,
           int cost, Color color, int costPerHouse);
    size_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
    void onPlayerEntry(Token token) override;
    std::vector<std::string> writeTileInfo() override;
    int numberOfHouses = 0;
    int costPerHouse;
    int tax;
};

class Utility final : public OwnableTile {
public:
    Utility(Board& board, int position, std::string name,
            int cost, Color color);
    std::vector<std::string> writeTileInfo() override;
    size_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
};

class Start final : public FieldTile {
public:
    Start(Board& board, int position, std::string name);
    std::vector<std::string> writeTileInfo() override;
    void onPlayerPass(Token token) override;
    void onPlayerEntry(Token token) override;
    void setMortgageCost(int val = 0) override { };
    bool MortgageTile(Token token) override { return false; };
};

class Prison final : public FieldTile {
public:
    Prison(Board& board, int position, std::string name);
    std::vector<std::string> writeTileInfo() override;
    void onPlayerEntry(Token token) override;
    void setMortgageCost(int val = 0) override { };
    bool MortgageTile(Token token) override { return false; };

    int tax = 50;
};

//Considered harmful
class GoToPrison final : public FieldTile {
public:
    GoToPrison(Board& board, int position, std::string name);
    std::vector<std::string> writeTileInfo() override;
    void onPlayerEntry(Token token) override;
    void setMortgageCost(int val = 0) override { };
    bool MortgageTile(Token token) override { return false; };
};

class Chance final : public FieldTile {
public:
    Chance(Board& board, int position, std::string name);
    std::vector<std::string> writeTileInfo() override;
    void onPlayerEntry(Token token) override;
    void setMortgageCost(int val = 0) override { };
    bool MortgageTile(Token token) override { return false; };

    std::vector<Card*> cards;
};

class PublicTreasury final : public FieldTile {
public:
    PublicTreasury(Board& board, int position, std::string name);
    void onPlayerEntry(Token token) override;
    std::vector<std::string> writeTileInfo() override;
    void setMortgageCost(int val = 0) override { };
    bool MortgageTile(Token token) override { return false; };

    std::vector<Card*> cards;
};

class IncomeTax final : public FieldTile {
public:
    IncomeTax(Board& board, int position, std::string name,
            int tax);
    std::vector<std::string> writeTileInfo() override;
    void onPlayerEntry(Token token) override;
    void setMortgageCost(int val = 0) override { };
    bool MortgageTile(Token token) override { return false; };

private:
    int tax;
};

class FreeParking final : public FieldTile {
public:
    FreeParking(Board& board, int position, std::string name);
    std::vector<std::string> writeTileInfo() override;
    void setMortgageCost(int val = 0) override { };
    bool MortgageTile(Token token) override { return false; };
};

void makeDefaultRequest(PlayerRequest& r);
bool handleGenericActions(Token token, const FieldTile& tile, const PlayerReply& reply);

#endif //FIELD_H
