#include "Cards.h"
#include <algorithm>
#include <random>

Card::Card(Board &board, std::string text) : board(board), text(std::move(text)) {}

CardPool::CardPool() {

}

CardPool::CardPool(Board& board) {
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
    static std::mt19937 g_gen(g_rd()); //TODO: rewrite with Random.h
    std::shuffle(pool.begin(), pool.end(), g_gen);
}

std::vector<std::unique_ptr<Card>> &CardPool::getCards() {
    return pool;
}

const std::vector<std::unique_ptr<Card>>& CardPool::getCards() const {
    return pool;
}
