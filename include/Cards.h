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

class CardPool {
public:
    CardPool(); // Тут создаются карточки

    const std::vector<std::unique_ptr<Card>>& getCards() const;
    std::unique_ptr<Card> takeCard();
    void returnCard(std::unique_ptr<Card> card);
    void shuffle();
private:
    std::vector<std::unique_ptr<Card>> pool;
};

#endif //CARDS_H
