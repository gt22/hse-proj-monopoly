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
    field[1]->color = COL1;

    field[2] = new Chance(*this, 2, "Public treasury");

    field[3] = new Street(*this, 3, "Nagatinskaya street");
    field[3]->cost = 60;
    field[3]->costPerHouse = 50;
    field[3]->color = COL1;

    field[4] = new IncomeTax(*this, 4, "Income tax");

    field[5] = new Railway(*this, 5, "Riga railway");
    field[5]->cost = 200;

    field[6] = new Street(*this, 6, "Varshavskoe highway");
    field[6]->cost = 100;
    field[6]->costPerHouse = 50;
    field[6]->color = COL2;

    field[7] = new Chance(*this, 7, "Chance");

    field[8] = new Street(*this, 8, "Ogareva street");
    field[8]->cost = 100;
    field[8]->costPerHouse = 50;
    field[8]->color = COL2;

    field[9] = new Street(*this, 9, "the 1st Park street");
    field[9]->cost = 120;
    field[9]->costPerHouse = 50;
    field[9]->color = COL2;

    field[10] = new Prison(*this, 10, "Prison");

    field[11] = new Street(*this, 11, "Polyanka street");
    field[11]->cost = 140;
    field[11]->costPerHouse = 100;
    field[11]->color = COL3;

    field[12] = new Utility(*this, 12, "Electric station");
    field[12]->cost = 150;

    field[13] = new Street(*this, 13, "Sretenka");
    field[13]->cost = 140;
    field[13]->costPerHouse = 100;
    field[13]->color = COL3;

    field[14] = new Street(*this, 14, "Rostov embankment");
    field[14]->cost = 160;
    field[14]->costPerHouse = 100;
    field[14]->color = COL3;

    field[15] = new Railway(*this, 15, "Kursk railway");
    field[15]->cost = 200;

    field[16] = new Street(*this, 16, "Ryazansky prospect");
    field[16]->cost = 180;
    field[16]->costPerHouse = 100;
    field[16]->color = COL4;

    field[17] = new IncomeTax(*this, 17, "Income tax");

    field[18] = new Street(*this, 18, "Vavilov street");
    field[18]->cost = 180;
    field[18]->costPerHouse = 100;
    field[18]->color = COL4;

    field[19] = new Street(*this, 19, "Rublevka");
    field[19]->cost = 200;
    field[19]->costPerHouse = 100;
    field[19]->color = COL4;

    field[20] = new FreeParking(*this, 20, "Free parking");

    field[21] = new Street(*this, 21, "Tverskaya street");
    field[21]->cost = 220;
    field[21]->costPerHouse = 150;
    field[21]->color = COL5;

    field[22] = new Chance(*this, 22, "Chance");

    field[23] = new Street(*this, 23, "Pushkinskaya street");
    field[23]->cost = 220;
    field[23]->costPerHouse = 150;
    field[23]->color = COL5;

    field[24] = new Street(*this, 24, "Mayakovsky square");
    field[24]->cost = 240;
    field[24]->costPerHouse = 150;
    field[24]->color = COL5;

    field[25] = new Railway(*this, 25, "Kazan railway");
    field[25]->cost = 200;

    field[26] = new Street(*this, 26, "Georgian Val street");
    field[26]->cost = 260;
    field[26]->costPerHouse = 150;
    field[26]->color = COL6;

    field[27] = new Street(*this, 27, "Tchaikovsky street");
    field[27]->cost = 260;
    field[27]->costPerHouse = 150;
    field[27]->color = COL6;

    field[28] = new Utility(*this, 28, "Water supply");
    field[28]->cost = 150;

    field[29] = new Street(*this, 29, "Smolensk square");
    field[29]->cost = 280;
    field[29]->costPerHouse = 150;
    field[29]->color = COL6;

    field[30] = new GoToPrison(*this, 30, "Go to prison");
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
    std::size_t newPoz = (position + throwSum) % Board::FIELD_SIZE;
    if (newPoz < position) {
        money += 200;
    }
    position = (position + throwSum) % Board::FIELD_SIZE;
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