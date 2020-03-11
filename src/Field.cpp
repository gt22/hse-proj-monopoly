#include "Field.h"
#include "Board.h"

FieldTile::FieldTile(Board& board, int position, std::string name) :
                        board(board), position(position), name(std::move(name)) {}

void FieldTile::onPlayerPass(Token) {}

void FieldTile::onPlayerEntry(Token) {}

void Start::onPlayerPass(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.money += 200;
}

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

void GoToPrison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.toPrison();
}

void Chance::onPlayerEntry(Token token) {
    board.getPlayer(token).cards.push_back(board.deck.takeCard());
}

void IncomeTax::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    if (player.money >= 200) {
        player.money -= 200;
        return;
    }
    player.toPrison();
}

void FreeParking::onPlayerPass(Token token) { return; }

void Railway::onPlayerEntry(Token token) {
    if (owner == token) {
        return;
    }
    if (owner == Token::FREE_FIELD) {
        //TODO: покупка поля/торги
        return;
    }
    PlayerData& player = board.getPlayer(token);
    //TODO: найти какой процент должен заплатить
}