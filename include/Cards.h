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
    virtual void onPlayerGet(Token token) = 0;
};

class LeftPrisonForFree : public Card {
    LeftPrisonForFree(Board& board, std::string text = "You can left prison for free");
    void apply(Token token) override;
    void onPlayerGet(Token token) override;
};

class GetMoney : public Card {
    GetMoney(Board& board, std::string text = "");
    void apply(Token token) override;
    void onPlayerGet(Token token) override;
    int amount = 50;
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
