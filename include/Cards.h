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
    virtual void setPos(int newPos) = 0;
};

class LeavePrisonForFree : public Card {
public:
    explicit LeavePrisonForFree(Board& board, std::string text = "You can leave prison for free");
    void apply(Token token) override;
    void setAmount(int val) override { }
    void setFlag(bool val) override { };
    void setPos(int newPos) override { };
};

class GetMoney : public Card {
public:
    explicit GetMoney(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override;
    void setFlag(bool val) override { };
    void setPos(int newPos) override { };
    int amount = 50;
};

class PayMoney : public Card {
public:
    explicit PayMoney(Board& board, std::string text = "");
    void apply(Token token) override;
    int countPlayerTax(const PlayerData &playerData) const;
    void setAmount(int val) override;
    void setFlag(bool val) override { countTax = val; };
    void setPos(int newPos) override { };
    int amount = 50;
    bool countTax = false;
};

class GetMoneyFromOtherPlayers : public Card {
public:
    explicit GetMoneyFromOtherPlayers(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override;
    void setFlag(bool val) override { };
    void setPos(int newPos) override { };
    int amount = 50;
};

class PayMoneyToOtherPlayers : public Card {
public:
    explicit PayMoneyToOtherPlayers(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override;
    void setFlag(bool val) override { };
    void setPos(int newPos) override { };
    int amount = 50;
};


class Teleport : public Card {
public:
    explicit Teleport(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override;
    void setFlag(bool val) override { fl = val; };
    void setPos(int newPos) override { newPosition = newPos; };
    int amount = 0;
    int newPosition;
    int deltaPos = -3;
    bool fl = false;
};

class TeleportToPrison : public Card {
public:
    explicit TeleportToPrison(Board& board, std::string text = "");
    void apply(Token token) override;
    void setAmount(int val) override { };
    void setFlag(bool val) override { };
    void setPos(int newPos) override { };
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
