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
    COL8,
    NO_COL
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
    virtual Token getOwner() const { return Token::FREE_FIELD; }
    virtual Color getColor() const { return Color::NO_COL; }
    virtual int getMortgageCost() const { return 0; }
    virtual int getNumberOfHouses() const { return 0; }
    virtual int getNumberOfHotels() const { return 0; }
    virtual int getHouseCost() const { return 0; }
    virtual int getHotelCost() const { return 0; }
    virtual void addHouse() { }
    virtual void addHotel() { }
    virtual void setTaxes(int tax0, int tax1, int tax2, int tax3, int tax4, int tax5) { }
    virtual void decrHotelNum() { }
    virtual void decrHouseNum() { }
    virtual void setOwner(Token token) { }
    virtual int getFieldCost() { return 0; }
    virtual void decrPropertyNum(Token token) { };
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
    void setOwner(Token token) override { owner = token; }
    int getMortgageCost() const override { return mortgageCost; }
    int getFieldCost() override { return cost; }
    int mortgageCost = 0;
    int cost;
    Color color;
    Token owner = Token::FREE_FIELD; //TODO: как обозначить отсутствие владельца
    int costOfParking = 0;
    int tax;
};

class Railway final : public OwnableTile {
public:
    Railway(Board& board, int position, std::string name,
            int cost, Color color);
    size_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
    std::vector<std::string> writeTileInfo() override;
    void decrPropertyNum(Token token) override;
};

class Street final : public OwnableTile {
public:
    Street(Board& board, int position, std::string name,
           int cost, Color color, int costPerHouse, int costPerHotel = 0);
    size_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
    std::vector<std::string> writeTileInfo() override;
    int getNumberOfHouses() const override { return numberOfHouses; }
    int getNumberOfHotels() const override { return numberOfHotels; }
    int getHotelCost() const override { return costPerHotel; }
    int getHouseCost() const override { return costPerHouse; }
    void addHouse() override { numberOfHouses++; }
    void addHotel() override { numberOfHotels++; }
    void decrHotelNum() override { numberOfHotels--; }
    void decrHouseNum() override { numberOfHouses--; }
    void setTaxes(int tax0, int tax1, int tax2, int tax3, int tax4, int tax5) override;
    int numberOfHouses = 0;
    int numberOfHotels = 0;
    int costPerHouse = 0;
    int costPerHotel = 0;

    int startTax;
    int taxOneHouse;
    int taxTwoHouses;
    int taxThreeHouses;
    int taxFourHouses;
    int taxHotel;
};

class Utility final : public OwnableTile {
public:
    Utility(Board& board, int position, std::string name,
            int cost, Color color);
    std::vector<std::string> writeTileInfo() override;
    size_t calculateTax(Token token) override;
    void onPurchase(Token token) override;
    void decrPropertyNum(Token token) override;
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
    int getTax() const;
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

void makeDefaultRequest(PlayerRequest& r, Token token, Board& board);
bool handleGenericActions(Token token, const FieldTile& tile, const PlayerReply& reply);

#endif //FIELD_H
