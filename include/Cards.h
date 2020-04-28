#ifndef CARDS_H
#define CARDS_H

#include "MonopolyFwd.h"
#include <string>
#include <vector>
#include <memory>

static constexpr int HOUSE_REPAIR_COST = 25;
static constexpr int HOTEL_REPAIR_COST = 100;

class Card {
public:
    Card(Board& board, std::string text);
    virtual ~Card() = default;
    Board& board;
    std::string text;
    virtual void apply(Token token) = 0;
    virtual void setAmount(int val) = 0;
    virtual void setFlag(bool val) = 0;
    virtual void setPos(int new_pos) = 0;
};

class LeftPrisonForFree : public Card {
public:
    LeftPrisonForFree(Board& board, std::string text = "You can left prison for free");
    void apply(Token token) override;
    void setAmount(int val) override { }
    void setFlag(bool val) override { };
    void setPos(int new_pos) override { };
};

class GetMoney : public Card {
public:
    GetMoney(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override;
    void setFlag(bool val) override { };
    void setPos(int new_pos) override { };
    int amount = 50;
};

class PayMoney : public Card {
public:
    PayMoney(Board& board, std::string text = "");
    void apply(Token token) override;
    int countPlayerTax(const PlayerData &playerData) const;
    void setAmount(int val) override;
    void setFlag(bool val) override { count_tax = val; };
    void setPos(int new_pos) override { };
    int amount = 50;
    bool count_tax = false;
};

class GetMoneyFromOtherPlayers : public Card {
public:
    GetMoneyFromOtherPlayers(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override;
    void setFlag(bool val) override { };
    void setPos(int new_pos) override { };
    int amount = 50;
};

class PayMoneyToOtherPlayers : public Card {
public:
    PayMoneyToOtherPlayers(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override;
    void setFlag(bool val) override { };
    void setPos(int new_pos) override { };
    int amount = 50;
};


class Teleport : public Card {
public:
    Teleport(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override;
    void setFlag(bool val) override { fl = val; };
    void setPos(int new_pos) override { new_position = new_pos; };
    int amount = 0;
    int new_position;
    int delta_pos = -3;
    bool fl = false;
};

class TeleportToPrison : public Card {
public:
    TeleportToPrison(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override { };
    void setFlag(bool val) override { };
    void setPos(int new_pos) override { };
};

class CardPool {
public:
    CardPool(); // Тут создаются карточки

    std::unique_ptr<Card> takeCard();
    void returnCard(std::unique_ptr<Card> card);
    void shuffle();
private:
    std::vector<std::unique_ptr<Card>> pool;
};

#endif //CARDS_H
