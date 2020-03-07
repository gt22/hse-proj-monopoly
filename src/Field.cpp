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
    board.getPlayer(token).daysLeftInPrison = 1;
}

void Chance::onPlayerEntry(Token token) {
    board.getPlayer(token).cards.push_back(board.deck.takeCard());
}
