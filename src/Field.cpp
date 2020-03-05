#include "Field.h"
#include "Board.h"

FieldTile::FieldTile(Board& board, int position, std::string name) :
                        board(board), position(position), name(std::move(name)){}

void FieldTile::onPlayerPass(Token) {
    return;
}

void FieldTile::onPlayerEntry(Token) {
    return;
}

void Start::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.money += 200;
}

void Start::onPlayerPass(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.money += 200;
}

bool throwDouble() {
    return true;
}

void Prison::onPlayerEntry(Token token) {
    static_cast<void>(token);
    return;
}