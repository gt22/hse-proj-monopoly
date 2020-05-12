#include "Cards.h"
#include "Field.h"

#include <algorithm>
#include <random>
#include <Board.h>
#include <iostream>

Card::Card(Board &board, std::string text) : board(board), text(std::move(text)) {}

LeavePrisonForFree::LeavePrisonForFree(Board &board, std::string text)
                 : Card(board, std::move(text)) {}

GetMoney::GetMoney(Board &board, std::string text)
        : Card(board, std::move(text)) {}

PayMoney::PayMoney(Board &board, std::string text)
        : Card(board, std::move(text)) {}

GetMoneyFromOtherPlayers::GetMoneyFromOtherPlayers(Board &board, std::string text)
                        : Card(board, std::move(text)) {}

PayMoneyToOtherPlayers::PayMoneyToOtherPlayers(Board &board, std::string text)
                        : Card(board, std::move(text)) {}


Teleport::Teleport(Board &board, std::string text)
        : Card(board, std::move(text)) {}

TeleportToPrison::TeleportToPrison(Board &board, std::string text)
                : Card(board, std::move(text)) {}

void LeavePrisonForFree::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.cardToLeavePrison = true;
}

void GetMoney::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.addMoney(amount);
}

void PayMoney::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request;
    FieldTile* cur_field = board.getTile(player.position);
    bool payTax = false;
    request.availableActions.push_back(PlayerAction::PAY_TAX);
    int tmp = amount;
    if (countTax) {
        amount = countPlayerTax(player);
    }
    while (true) {
        makeDefaultRequest(request, token, board);
        PlayerReply reply = board.sendRequest(player.token, request);
        board.sync();
        request.message = "";
        if (reply->action == PlayerAction::END_TURN && !payTax) {
            request.message = "You can't finish turn now";
            continue;
        }
        if (reply->action == PlayerAction::PAY_TAX) {
            if (player.getMoney() >= amount) {
                player.addMoney(-amount);
                amount = tmp;
                return;
            } else {
                request.message = "You don't have enough money";
                continue;
            }
        }
        if (!handleGenericActions(token, (*cur_field), reply)) {
            amount = tmp;
            return;
        }
    }
}

int PayMoney::countPlayerTax(const PlayerData &player) const {
    return player.numberOfHotels * HOTEL_REPAIR_COST + player.numberOfHouses * HOUSE_REPAIR_COST;
}

void GetMoneyFromOtherPlayers::apply(Token token) {
    PayMoney * payMoney = new PayMoney(board);
    PlayerData& player = board.getPlayer(token);
    for (std::size_t i = 0; i < board.getPlayers().size(); i++) {
        if (!board.getPlayers()[i].alive) {
            continue;
        }
        if (board.getPlayers()[i].token == token) {
            continue;
        }
        board.sendMessage(board.getPlayers()[i].token, PlayerMessage("You must pay " + std::to_string(amount)
                                                              + " to " + std::string(board.getPlayers()[i].name)),
                                                       MessageType::INFO);
        board.sync();
        payMoney->apply(board.getPlayers()[i].token);
        player.addMoney(amount);
    }
}

void PayMoneyToOtherPlayers::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request;
    FieldTile* cur_field = board.getTile(player.position);
    bool payTax = false;
    request.availableActions.push_back(PlayerAction::PAY_TAX);
    int tmp = amount;
    amount = board.getCurNumOfPlayers() * tmp;
    while (true) {
        makeDefaultRequest(request, token, board);
        PlayerReply reply = board.sendRequest(player.token, request);
        board.sync();
        request.message = "";
        if (reply->action == PlayerAction::END_TURN && !payTax) {
            request.message = "You can't finish turn now";
            continue;
        }
        if (reply->action == PlayerAction::PAY_TAX) {
            if (player.getMoney() >= amount) {
                player.addMoney(-amount);
                amount = tmp;
                for (std::size_t i = 0; i < board.getPlayers().size(); i++) {
                    if (!board.getPlayers()[i].alive) {
                        continue;
                    }
                    if (board.getPlayers()[i].token == token) {
                        continue;
                    }
                    board.getPlayer(board.getPlayers()[i].token).addMoney(tmp);
                    board.sync();
                }
                return;
            } else {
                request.message = "You don't have enough money";
                continue;
            }
        }
        if (!handleGenericActions(token, (*cur_field), reply)) {
            amount = tmp;
            return;
        }
    }
}

void Teleport::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    if (fl) {
        player.newPosition(deltaPos);
    } else {
        player.newPosition(newPosition - (int)player.position);
    }
    player.addMoney(amount);
}

void TeleportToPrison::apply(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.toPrison();
}

void GetMoney::setAmount(int val) {
    amount = val;
}

void PayMoney::setAmount(int val) {
    amount = val;
}

void GetMoneyFromOtherPlayers::setAmount(int val) {
    amount = val;
}

void PayMoneyToOtherPlayers::setAmount(int val) {
    amount = val;
}

void Teleport::setAmount(int val) {
    amount = val;
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