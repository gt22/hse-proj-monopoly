#include "Cards.h"
#include <algorithm>
#include <random>

Card::Card(Board &board, std::string text) : board(board), text(std::move(text)) {}

LeftPrisonForFree::LeftPrisonForFree(Board &board, std::string text)
                 : Card(board, std::move(text)) {}

GetMoney::GetMoney(Board &board, std::string text)
        : Card(board, std::move(text)) {}

void LeftPrisonForFree::onPlayerGet(Token token) { }

void LeftPrisonForFree::apply(Token token) { }

void GetMoney::onPlayerGet(Token token) { }

void GetMoney::apply(Token token) { }

CardPool::CardPool() {
    //pool.push_back(std::make_unique(...))
}

std::unique_ptr<Card> CardPool::takeCard() {
    auto card = std::move(pool.back());
    pool.pop_back();
    return card;
}

void CardPool::returnCard(std::unique_ptr<Card> card) {
    pool.push_back(std::move(card));
}

void CardPool::shuffle() {
    static std::random_device g_rd;
    static std::mt19937 g_gen(g_rd());
    std::shuffle(pool.begin(), pool.end(), g_gen);
}