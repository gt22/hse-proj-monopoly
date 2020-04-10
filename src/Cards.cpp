#include "Cards.h"
#include <algorithm>
#include <random>
#include <Board.h>

Card::Card(Board &board, std::string text) : board(board), text(std::move(text)) {}

LeftPrisonForFree::LeftPrisonForFree(Board &board, std::string text)
                 : Card(board, std::move(text)) {}

GetMoney::GetMoney(Board &board, std::string text)
        : Card(board, std::move(text)) {}

PayMoney::PayMoney(Board &board, std::string text)
        : Card(board, std::move(text)) {}

GetMoneyFromOtherPlayers::GetMoneyFromOtherPlayers(Board &board, std::string text)
                        : Card(board, std::move(text)) {}

Teleport::Teleport(Board &board, std::string text)
        : Card(board, std::move(text)) {}

TeleportToPrison::TeleportToPrison(Board &board, std::string text)
                : Card(board, std::move(text)) {}

void LeftPrisonForFree::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.cardToLeavePrison = true;
}

void GetMoney::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.addMoney(amount);
}

void PayMoney::apply(Token token) {
    //TODO
}

void GetMoneyFromOtherPlayers::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    for (std::size_t i = 0; i < board.getPlayers().size(); i++) {
        if (board.getPlayers()[i].token == token) {
            continue;
        }
        board.sendMessage(board.getPlayers()[i].token, PlayerMessage("You must pay " + std::to_string(amount)
                                                              + " to " + std::string(board.getPlayers()[i].name)));
        //TODO:
        player.addMoney(amount);
    }
}

void Teleport::apply(Token token) {
    //TODO
}

void TeleportToPrison::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.toPrison();
}

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