#include "Board.h"

#include <utility>
#include "Field.h"
#include "Game.h"

PlayerData::PlayerData(std::string_view name, Token token) : name(name), token(token) {}

int PlayerData::getMoney() {
    return money;
}

int PlayerData::addMoney(int newMoney) {
    money += newMoney;
    return money;
}

void PlayerData::setLoser() {
    alive = false;
    //TODO:
}

void PlayerData::newPosition(int throwSum) {
    std::size_t newPos = (position + throwSum + Board::FIELD_SIZE) % Board::FIELD_SIZE;
    if (newPos < position) {
        (*this).addMoney(START_SUM);
    }
    position = (position + throwSum + Board::FIELD_SIZE) % Board::FIELD_SIZE;
}

void PlayerData::toPrison() {
    position = PRISON_POS;
    daysLeftInPrison = 3;
    doubleDice = 0;
    prisoner = true;
}

void PlayerData::outOfPrison() {
    prisoner = false;
    daysLeftInPrison = 0;
}

Board::Board(const std::vector<std::pair<std::string_view, Token> > & playersList, Game& game) : field(), deck(), game(game) {
    players.reserve(playersList.size());
    for (auto [name, token] : playersList) {
        players.emplace_back(name, token);
    }
    numOfAlivePlayers = playersList.size();
    //TODO: Fill field with tiles
    field[0] = new Start(*this, 0, "Start");

    field[1] = new Street(*this, 1, "Zhitnaya street",
            60, Color::COL1, 50);
    field[1]->setMortgageCost(30);

    field[2] = new PublicTreasury(*this, 2, "Public treasury");


    field[3] = new Street(*this, 3, "Nagatinskaya street",
            60, Color::COL1, 50);
    field[3]->setMortgageCost(30);

    field[4] = new IncomeTax(*this, 4, "Income tax", 200);

    field[5] = new Railway(*this, 5, "Riga railway",
            200, Color::COL1);
    field[5]->setMortgageCost(100);

    field[6] = new Street(*this, 6, "Varshavskoe highway",
            100, Color::COL2, 50);
    field[6]->setMortgageCost(50);

    field[7] = new Chance(*this, 7, "Chance");

    field[8] = new Street(*this, 8, "Ogareva street",
            100, Color::COL2, 50);
    field[8]->setMortgageCost(50);

    field[9] = new Street(*this, 9, "the 1st Park street",
            120, Color::COL2, 50);
    field[9]->setMortgageCost(60);

    field[10] = new Prison(*this, 10, "Prison");

    field[11] = new Street(*this, 11, "Polyanka street",
            140, Color::COL3, 100);
    field[11]->setMortgageCost(70);

    field[12] = new Utility(*this, 12, "Electric station",
            150, Color::COL1);
    field[12]->setMortgageCost(75);

    field[13] = new Street(*this, 13, "Sretenka",
            140, Color::COL3, 100);
    field[13]->setMortgageCost(70);

    field[14] = new Street(*this, 14, "Rostov embankment",
            160, Color::COL3, 100);
    field[14]->setMortgageCost(80);

    field[15] = new Railway(*this, 15, "Kursk railway",
            200, Color::COL1);
    field[15]->setMortgageCost(100);

    field[16] = new Street(*this, 16, "Ryazansky prospect",
            180, Color::COL4, 100);
    field[16]->setMortgageCost(90);

    field[17] = new PublicTreasury(*this, 17, "Public treasury");

    field[18] = new Street(*this, 18, "Vavilov street",
            180, Color::COL4, 100);
    field[18]->setMortgageCost(90);

    field[19] = new Street(*this, 19, "Rublevka",
            200, Color::COL4, 100);
    field[19]->setMortgageCost(100);

    field[20] = new FreeParking(*this, 20, "Free parking");

    field[21] = new Street(*this, 21, "Tverskaya street",
            220, Color::COL5, 150);
    field[21]->setMortgageCost(110);

    field[22] = new Chance(*this, 22, "Chance");

    field[23] = new Street(*this, 23, "Pushkinskaya street",
            220, Color::COL5, 150);
    field[23]->setMortgageCost(110);

    field[24] = new Street(*this, 24, "Mayakovsky square",
            240, Color::COL5, 150);
    field[24]->setMortgageCost(120);

    field[25] = new Railway(*this, 25, "Kazan railway",
            200, Color::COL1);
    field[25]->setMortgageCost(100);

    field[26] = new Street(*this, 26, "Georgian Val street",
            260, Color::COL6, 150);
    field[26]->setMortgageCost(130);

    field[27] = new Street(*this, 27, "Tchaikovsky street",
            260, Color::COL6, 150);
    field[27]->setMortgageCost(130);

    field[28] = new Utility(*this, 28, "Water supply",
            150, Color::COL1);
    field[28]->setMortgageCost(75);

    field[29] = new Street(*this, 29, "Smolensk square",
            280, Color::COL6, 150);
    field[29]->setMortgageCost(140);

    field[30] = new GoToPrison(*this, 30, "Go to prison");

    field[31] = new Street(*this, 31, "Shchusev street",
            300, Color::COL7, 200);
    field[31]->setMortgageCost(150);

    field[32] = new Street(*this, 32, "Gogolevsky boulevard",
            300, Color::COL7, 200);
    field[32]->setMortgageCost(150);

    field[33] = new PublicTreasury(*this, 33, "Public treasury");

    field[34] = new Street(*this, 34, "Kutuzovsky prospect",
            320, Color::COL7, 200);
    field[34]->setMortgageCost(160);

    field[35] = new Railway(*this, 35, "Leningrad railway",
            200, Color::COL1);
    field[35]->setMortgageCost(100);

    field[36] = new Chance(*this, 36, "Chance");

    field[37] = new Street(*this, 37, "Malaya Bronnaya",
            350, Color::COL8, 200);
    field[37]->setMortgageCost(175);

    field[38] = new IncomeTax(*this, 38, "Income tax", 100);

    field[39] = new Street(*this, 39, "Arbat street",
            400, Color::COL8, 200);
    field[39]->setMortgageCost(200);

    getMortgageToken[Token::DOG] = TokenForMortgage::DOG_M;
    getMortgageToken[Token::CAR] = TokenForMortgage::CAR_M;
    getMortgageToken[Token::HAT] = TokenForMortgage::HAT_M;
    getMortgageToken[Token::BOOT] = TokenForMortgage::BOOT_M;
    getMortgageToken[Token::CAT] = TokenForMortgage::CAT_M;
    getMortgageToken[Token::SHIP] = TokenForMortgage::SHIP_M;
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

const PlayerData &Board::getPlayer(Token token) const {
    for (auto &player : players) {
        if (player.token == token) {
            return player;
        }
    }
    //TODO: что вернуть если не нашли.
    throw 1;
}

Token Board::getPlayerToken(std::size_t index) const {
    return players[index].token;
}

PlayerReply Board::sendRequest(Token token, PlayerRequest request) const {
    return game.sendRequest(token, std::move(request));
}

void Board::sendMessage(Token token, PlayerMessage mes) const {
    game.sendMessage(token, std::move(mes));
}

std::size_t Board::getPlayersNumber() const {
    return players.size();
}

const std::vector<PlayerData> &Board::getPlayers() const {
    return players;
}

void Board::decrNumOfOlayers() {
    numOfAlivePlayers--;
}

bool Board::isFinished() const {
    return (numOfAlivePlayers <= 1) || terminated;
}

int Board::getCurNumOfPlayers() const {
    return numOfAlivePlayers;
}

Token Board::getWinner() const {
    for (std::size_t i = 0; i < (*this).getPlayersNumber(); i++) {
        if (players[i].alive) {
            return players[i].token;
        }
    }
    //TODO: что вернуть если не нашли.
    throw 1;
}

FieldTile *Board::getTile(std::size_t pos) const {
    return field[pos];
}

void Board::sync() {
    game.sync();
}

void Board::terminate() {
    terminated = true;
}

std::size_t Board::getPlayerIndex() const {
    return index;
}

void Board::setPlayerIndex(std::size_t num) {
    index = num;
}

FieldTile *Board::getFieldTile(int ind) {
    if (ind < 0 || ind > (int)FIELD_SIZE) {
        throw std::out_of_range("no field " + std::to_string(ind));
    }
    return field[ind];
}

bool Board::checkAllFieldsOfCurColor(Token token, int ind) const {
    if (ind < 0 || ind > (int)FIELD_SIZE) {
        throw std::out_of_range("no field " + std::to_string(ind));
    }
    for (std::size_t i = 0; i < FIELD_SIZE; i++) {
        if (field[i]->getColor() == field[ind]->getColor() &&
            field[i]->getOwner() != field[ind]->getOwner()) {
            return false;
        }
    }
    return true;
}

int countPrevForColor(int ind, const Board& board) {
    if (ind < 0 || ind > (int)board.FIELD_SIZE) {
        throw std::out_of_range("no field " + std::to_string(ind));
    }
    int cnt = 0;
    for (int i = ind; i >= 0; i--) {
        if (board.field[i]->getColor() == board.field[ind]->getColor()) {
            cnt++;
        }
    }
    return cnt;
}

bool checkHousesNumForHotel(int ind, const Board& board) {
    for (std::size_t i = ind; i < board.FIELD_SIZE; i++) {
        if (board.field[i]->getColor() == board.field[ind]->getColor() &&
                (board.field[i]->getNumberOfHouses() != 4 || board.field[i]->getNumberOfHotels() != 0)) {
            return false;
        }
    }
    return true;
}

bool checkPrevHotel(int ind, const Board& board) {
    int i = ind - 1;
    while (i >= 0) {
        if (board.field[i]->getColor() == board.field[ind]->getColor()
            && board.field[i]->getNumberOfHotels() == 1) {
            return true;
        }
        i--;
    }
    return false;
}

bool checkPrevHouse(int ind, const Board& board) {
    int i = ind - 1;
    while (i >= 0) {
        if (board.field[i]->getColor() == board.field[ind]->getColor()
            && board.field[i]->getNumberOfHouses() == board.field[ind]->getNumberOfHouses() + 1) {
            return true;
        }
        i--;
    }
    return false;
}

bool checkHousesNumForHouse(int ind, const Board& board) {
    for (std::size_t i = ind; i < board.FIELD_SIZE; i++) {
        if (board.field[i]->getColor() == board.field[ind]->getColor() &&
            (board.field[i]->getNumberOfHouses() !=  board.field[i]->getNumberOfHotels())) {
            return false;
        }
    }
    return true;
}

bool checkPrevForHotel(int ind, const Board& board) {
    int pos = countPrevForColor(ind, board);
    if (pos == 0) {
        return checkHousesNumForHotel(ind, board);
    }
    return checkPrevHotel(ind, board);
}

bool checkPrevForHouse(int ind, const Board& board) {
    int pos = countPrevForColor(ind, board);
    if (pos == 0) {
        return checkHousesNumForHouse(ind, board);
    }
    return checkPrevHouse(ind, board);
}

