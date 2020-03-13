#include "Field.h"

#include <utility>
#include "Board.h"

FieldTile::FieldTile(Board& board, int position, std::string name) :
                        board(board), position(position), name(std::move(name)) {}

void FieldTile::onPlayerPass(Token) {}

void FieldTile::onPlayerEntry(Token) {}

void Start::onPlayerPass(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.money += 200;
}

Start::Start(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}

void Prison::onPlayerEntry(Token token) {
    if (!board.getPlayer(token).prisoner) {
        return;
    }
    if (board.sendRequest(token, PlayerRequest("Would you like to pay 50000 to leave prison?")).answer) {
        board.getPlayer(token).money -= PRISON_FINE;
    }
    //ask to pay fine 50
    //ask to use special card
    return;
}

Prison::Prison(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}

void GoToPrison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.toPrison();
}

GoToPrison::GoToPrison(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}

void Chance::onPlayerEntry(Token token) {
    board.getPlayer(token).cards.push_back(board.deck.takeCard());
}

Chance::Chance(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}

void IncomeTax::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    //check property
    if (player.money < tax) {
        // aks other players if they want to buy smth(???)
    }
    while (player.money < tax) {
        if (!board.sendRequest(token, PlayerRequest("You don't have enough money.\n Would you like to mortgage real estate?")).answer) {
            break;
        }
        //what do you want to mortgage?
    }
    if (player.money >= tax) {
        player.money -= tax;
        return;
    }
    //lose
}

IncomeTax::IncomeTax(Board &board, int position, std::string name, int tax)
 : FieldTile(board, position, std::move(name)), tax(tax) {}

void FreeParking::onPlayerPass(Token token) { static_cast<void>(token); }

FreeParking::FreeParking(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}

void Railway::onPlayerEntry(Token token) {
    if (owner == token) {
        return;
    }
    PlayerData& player = board.getPlayer(token);
    if (owner == Token::FREE_FIELD) {
        if (board.sendRequest(token, PlayerRequest("Would you like to buy" + this->name + "?")).answer) {
            if (player.money >= cost) {
                player.money -= cost;
                (*this).owner = player.token;
                return;
            }
            //sendMessage "You don't have enough money"
        }
        //TODO: покупка торги
        return;
    }
    PlayerData& fieldOwner = board.getPlayer(owner);
    int tax = 25;
    for (int i = 1; i <= fieldOwner.numberOfRailways; i++) {
        tax *= 2;
    }
    static_cast<void>(player);
    //TODO: проверить, что может заплатить
}

OwnableTile::OwnableTile(Board &board, int position, std::string name, int cost, Color color)
 : FieldTile(board, position, std::move(name)), cost(cost), color(color) {}

Railway::Railway(Board &board, int position, std::string name, int cost, Color color)
 : OwnableTile(board, position, std::move(name), cost, color) {}

Street::Street(Board &board, int position, std::string name, int cost, Color color, int costPerHouse)
 : OwnableTile(board, position, std::move(name), cost, color), costPerHouse(costPerHouse) {}

Utility::Utility(Board &board, int position, std::string name, int cost, Color color)
 : OwnableTile(board, position, std::move(name), cost, color) {}
