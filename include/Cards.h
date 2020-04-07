#ifndef CARDS_H
#define CARDS_H

#include "MonopolyFwd.h"
#include <string>
#include <vector>
#include <memory>
class Card {
public:
    Card(Board& board, std::string text);
    virtual ~Card() = default;
    Board& board;
    std::string text;
    virtual void apply(Token token) = 0;
};

class LeftPrisonForFree : public Card {
public:
    LeftPrisonForFree(Board& board, std::string text = "You can left prison for free");
    void apply(Token token) override;
};

class GetMoney : public Card {
public:
    GetMoney(Board& board, std::string text = "");
    void apply(Token token) override;
    int amount = 50;
};

class PayMoney : public Card {
public:
    PayMoney(Board& board, std::string text = "");
    void apply(Token token) override;
    int amount = 50;
};

class GetMoneyFromOtherPlayers : public Card {
public:
    GetMoneyFromOtherPlayers(Board& board, std::string text = "");
    void apply(Token token) override;
    int amount = 50;
};

class Teleport : public Card {
public:
    Teleport(Board& board, std::string text = "");
    void apply(Token token) override;
    int amount = 50;
};

class TeleportToPrison : public Card {
public:
    TeleportToPrison(Board& board, std::string text = "");
    void apply(Token token) override;
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
