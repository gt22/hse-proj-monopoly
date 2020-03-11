#include "Board.h"

PlayerData::PlayerData(std::string_view name, Token token) : name(name), token(token) {}

Board::Board(const std::vector<std::pair<std::string_view, Token> > & playersList, Game& game) : field(), deck(), game(game) {
    players.reserve(playersList.size());
    for (auto [name, token] : playersList) {
        players.emplace_back(name, token);
    }
    //TODO: Fill field with tiles
    field[0] = new Start(*this, 0, "Start");

    field[1] = new Street(*this, 1, "Zhitnaya street");
    field[1]->cost = 60;
    field[1]->costPerHouse = 50;

    field[2] = new Chance(*this, 2, "Public treasury");

    field[3] = new Street(*this, 3, "Nagatinskaya street");
    field[3]->cost = 60;
    field[3]->costPerHouse = 50;

    field[4] = new IncomeTax(*this, 4, "Income tax");

    field[5] = new Railway(*this, 5, "Riga railway");
    field[5]->cost = 200;

    field[6] = new Street(*this, 6, "Varshavskoe highway");
    field[6]->cost = 100;
    field[6]->costPerHouse = 50;

    field[7] = new Chance(*this, 7, "Chance");

    field[8] = new Street(*this, 8, "Ogareva street");
    field[8]->cost = 100;
    field[8]->costPerHouse = 50;

    field[9] = new Street(*this, 9, "the 1st Park street");
    field[9]->cost = 100;
    field[9]->costPerHouse = 50;

    field[10] = new Prison(*this, 10, "Prison");
}

PlayerData& Board::getPlayer(Token token) {
    for (auto &player : players) {
        if (player.token == token) {
            return player;
        }
    }
    //TODO: что вернуть если не нашли.
    throw 1;
}

std::size_t Board::getPlayersNumber() {
    return players.size();
}

Token Board::getPlayerToken(std::size_t index) {
    return players[index].token;
}

void PlayerData::newPosition(std::size_t throwSum) {
    std::size_t newPoz = (position + throwSum) % 40;
    if (newPoz < position) {
        money += 200;
    }
    position = (position + throwSum) % 40;
}

void PlayerData::toPrison() {
    position = PRISON_POZ;
    daysLeftInPrison = 3;
    doubleDice = 0;
    prisoner = true;
}

PlayerReply Board::sendRequest(Token token, PlayerRequest request) {
    return game.sendRequest(token, request);
}