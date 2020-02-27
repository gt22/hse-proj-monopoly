#include "Field.h"
#include "Board.h"

int random_() {
    static int seed = 999546;
    seed = seed * 379276 + 3534227;
    return seed;
}

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
    int firstThrow = random_() % 6 + 1;
    int secondThrow = random_() % 6 + 1;
    return (firstThrow == secondThrow);
}

void Prison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.skip = true;
    if (throwDouble()) {
        //player.daysLeftInPrison = std::min(0, player.daysLeftInPrison);//???
        player.skip = false;
        return;
    }
    //else check if player wants to use special card
    if (player.cardToLeftPrison) {
        player.cardToLeftPrison = false;
        //player.daysLeftInPrison = 0;//???
        player.skip = false;
    }
}