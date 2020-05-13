#include "Field.h"
#include "Board.h"
#include "Random.h"
#include "Cards.h"

#include <utility>
#include <set>
#include <cassert>

#include <iostream>

FieldTile::FieldTile(Board& board, int position, std::string name) :
                        board(board), position(position), name(std::move(name)) {}

Start::Start(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {}

Prison::Prison(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {}

GoToPrison::GoToPrison(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {}

Chance::Chance(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {
    cards.resize(16);
    cards[0] = new Teleport(board, "Card #1");
    cards[0]->setPos(ARBAT_POS);

    cards[1] = new Teleport(board, "Card #2");
    cards[1]->setPos(RIGA_RAILWAY_POS);

    cards[2] = new GetMoney(board, "Card #3");
    cards[2]->setAmount(150);

    cards[3] = new Teleport(board, "Card #4");
    cards[3]->setPos(MAYAKOVSKY_SQUARE);

    //TODO: cards[4]
    cards[4] = new Teleport(board, "Card #5");
    cards[4]->setPos(MAYAKOVSKY_SQUARE);
    //TODO: cards[4]

    cards[5] = new TeleportToPrison(board, "Card #6");

    cards[6] = new Teleport(board, "Card #7");
    cards[6]->setPos(START_POS);

    cards[7] = new GetMoney(board, "Card #8");
    cards[7]->setAmount(50);

    cards[8] = new PayMoney(board, "Card #9");
    cards[8]->setFlag(true);

    cards[9] = new LeavePrisonForFree(board, "Card #10");

    cards[10] = new Teleport(board, "Card #11");
    cards[10]->setPos(POLYANKA_POS);

    //TODO: fill vector cards!!!
    cards[11] = new GetMoney(board, "Card #12");//50
    cards[11]->setAmount(50);

    cards[12] = new GetMoney(board, "Card #13");//10
    cards[12]->setAmount(10);

    cards[13] = new PayMoney(board, "Card #14");//30*buildings + 115*hotels
    cards[13]->setFlag(true);

    cards[14] = new Teleport(board, "Card #15");//to start
    cards[14]->setPos(START_POS);

    cards[15] = new GetMoney(board, "Card #16");//200
    cards[15]->setAmount(200);
    //TODO: fill vector cards!!!
}

PublicTreasury::PublicTreasury(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {
    cards.resize(16);

    cards[0] = new LeavePrisonForFree(board, "Card #1");

    cards[1] = new PayMoney(board, "Card #2");//50
    cards[1]->setAmount(50);

    cards[2] = new GetMoney(board, "Holiday pay! Get 100");//100
    cards[2]->setAmount(100);

    cards[3] = new GetMoney(board, "Card #4");//100
    cards[3]->setAmount(100);

    cards[4] = new PayMoney(board, "Card #5");//50
    cards[4]->setAmount(50);

    cards[5] = new GetMoney(board, "Card #6");//20
    cards[5]->setAmount(20);

    cards[6] = new GetMoney(board, "Card #7");//100
    cards[6]->setAmount(100);

    cards[7] = new TeleportToPrison(board, "Card #8");

    cards[8] = new GetMoney(board, "Card #9");//25
    cards[8]->setAmount(25);

    cards[9] = new GetMoneyFromOtherPlayers(board, "Card #10");//10
    cards[9]->setAmount(10);

    cards[10] = new GetMoney(board, "Card #11");//100
    cards[10]->setAmount(100);

    cards[11] = new GetMoney(board, "Card #12");//50
    cards[11]->setAmount(50);

    cards[12] = new GetMoney(board, "Card #13");//10
    cards[12]->setAmount(10);

    cards[13] = new PayMoney(board, "Card #14");//30*buildings + 115*hotels
    cards[13]->setFlag(true);

    cards[14] = new Teleport(board, "Card #15");//to start
    cards[14]->setPos(START_POS);

    cards[15] = new GetMoney(board, "Card #16");//200
    cards[15]->setAmount(200);
    //TODO: fill vector cards
}

IncomeTax::IncomeTax(Board &board, int position, std::string name, int tax)
        : FieldTile(board, position, std::move(name)), tax(tax) {}

FreeParking::FreeParking(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {}

OwnableTile::OwnableTile(Board &board, int position, std::string name, int cost, Color color)
        : FieldTile(board, position, std::move(name)), cost(cost), color(color) {}

Railway::Railway(Board &board, int position, std::string name, int cost, Color color)
        : OwnableTile(board, position, std::move(name), cost, color) { mortgageCost = 100; }

Street::Street(Board &board, int position, std::string name, int cost, Color color,
                    int costPerHouse, int costPerHotel)
        : OwnableTile(board, position, std::move(name), cost, color),
                costPerHouse(costPerHouse), costPerHotel(costPerHotel) {}

size_t Street::calculateTax(Token token) {
    switch (numberOfHouses + numberOfHotels) {
        case 0: return startTax;
        case 1: return taxOneHouse;
        case 2: return taxTwoHouses;
        case 3: return taxThreeHouses;
        case 4: return taxFourHouses;
        case 5: return taxHotel;
    }
    throw std::domain_error("too many buildings");
}

void Street::onPurchase(Token token) {
    // TODO
    return;
}

Utility::Utility(Board &board, int position, std::string name, int cost, Color color)
        : OwnableTile(board, position, std::move(name), cost, color) { mortgageCost = 75; }

void FieldTile::onPlayerPass(Token) {}

void FieldTile::onPlayerEntry(Token) {

    //TODO: generic actions?

}

void Start::onPlayerPass(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.addMoney(START_SUM);
}

template<typename T, typename C>
void addAll(std::vector<T>& vec, const C& cont) {
    std::copy(cont.begin(), cont.end(), std::back_inserter(vec));
}

void makeDefaultRequest(PlayerRequest& r, Token token, Board& board) {
    //TODO: New allocation each time... Maybe rewrite as clear, reserve & many push_backs?
    r.availableActions = {
            PlayerAction::LOSE,
            PlayerAction::END_TURN,
            PlayerAction::START_TRADE,
            PlayerAction::EXIT_GAME
    };
    const PlayerData& player = board.getPlayer(token);
    if (player.numberOfHotels > 0) {
        r.availableActions.push_back(PlayerAction::SELL_HOTEL);
    }
    if (player.numberOfHouses > 0) {
        //TODO: CHECK HOTELS
        r.availableActions.push_back(PlayerAction::SELL_HOUSE);
    }
    if (board.checkFieldWithoutBuildings(token)) {
        r.availableActions.push_back(PlayerAction::SELL_FIELD);
        r.availableActions.push_back(PlayerAction::MORTGAGE_HOLDINGS);
    }
    if (board.checkAllFieldsOfCol(token)) {
        //TODO: check
        r.availableActions.push_back(PlayerAction::BUY_HOUSE);
        r.availableActions.push_back(PlayerAction::BUY_HOTEL);
    }
}

bool handleGenericActions(Token token, const FieldTile& tile, const PlayerReply& reply) {
    if (reply->action == PlayerAction::LOSE) {
        PlayerData& player = tile.board.getPlayer(token);
        player.setLoser();
        tile.board.decrNumOfOlayers();
        tile.board.sendMessage(token, PlayerMessage("lose"), MessageType::INFO);
        //TODO
        return false;
    }
    if (reply->action == PlayerAction::BUY_HOUSE) {
        //TODO:send request for number of field
        NumReply numReply = tile.board.sendNumRequest(token);
        std::cout << numReply->num << "\n";
        int index = numReply->num;
        auto chosenField = tile.board.getFieldTile(index);
        if (token != chosenField->getOwner() || chosenField->getColor() == Color::NO_COL || chosenField->isMortgaged ||
            chosenField->getNumberOfHouses() >= 4 || chosenField->getNumberOfHotels() > 0) {
            tile.board.sendMessage(token, PlayerMessage("You can't build house on this field tile"), MessageType::INFO);
            return true;
        }
        if (!tile.board.checkAllFieldsOfCurColor(token, index)) {
            tile.board.sendMessage(token, PlayerMessage("You can't build house on this field tile"), MessageType::INFO);
            return true;
        }
        if (!checkPrevForHouse(index, tile.board)) {
            tile.board.sendMessage(token, PlayerMessage("You can't build house on this field tile"), MessageType::INFO);
            return true;
        }
        PlayerData& player = chosenField->board.getPlayer(token);
        if (player.getMoney() >= chosenField->getHouseCost()) {
            player.addMoney(-chosenField->getHouseCost());
            player.numberOfHouses++;
            chosenField->addHouse();
        } else {
            tile.board.sendMessage(token, PlayerMessage("You don't have enough money :("), MessageType::INFO);
        }
        return true;
    }
    if (reply->action == PlayerAction::BUY_HOTEL) {
        //TODO:send request for number of field
        NumReply numReply = tile.board.sendNumRequest(token);
        std::cout << numReply->num << "\n";
        int index = numReply->num;
        auto chosenField = tile.board.getFieldTile(index);
        if (token != chosenField->getOwner() || chosenField->getColor() == Color::NO_COL || chosenField->isMortgaged ||
                chosenField->getNumberOfHouses() < 4 || chosenField->getNumberOfHotels() > 0) {
            tile.board.sendMessage(token, PlayerMessage("You can't build hotel on this field tile"), MessageType::INFO);
            return true;
        }
        if (!tile.board.checkAllFieldsOfCurColor(token, index)) {
            tile.board.sendMessage(token, PlayerMessage("You can't build hotel on this field tile"), MessageType::INFO);
            return true;
        }
        if (!checkPrevForHotel(index, tile.board)) {
            tile.board.sendMessage(token, PlayerMessage("You can't build hotel on this field tile"), MessageType::INFO);
            return true;
        }
        PlayerData& player = tile.board.getPlayer(token);
        if (player.getMoney() >= chosenField->getHotelCost()) {
            player.addMoney(-chosenField->getHotelCost());
            player.numberOfHotels++;
            chosenField->addHotel();
        } else {
            tile.board.sendMessage(token, PlayerMessage("You don't have enough money :("), MessageType::INFO);
        }
        return true;
    }
    if (reply->action == PlayerAction::MORTGAGE_HOLDINGS) {
        //TODO:send request for number of field
        NumReply numReply = tile.board.sendNumRequest(token);
        std::cout << numReply->num << "\n";
        int index = numReply->num;
        if (tile.board.field[index]->getNumberOfHouses() != 0 && tile.board.field[index]->getNumberOfHotels() != 0) {
            tile.board.sendMessage(token, PlayerMessage("You can't mortgage this field tile"), MessageType::INFO);
            return true;
        }
        if (!tile.board.field[index]->isMortgaged && tile.board.field[index]->getOwner() == token) {
            PlayerData& player = tile.board.getPlayer(token);
            player.addMoney(tile.getMortgageCost());
            player.numberOfMortgagedProperty++;
            tile.board.field[index]->isMortgaged = true;
        } else {
            tile.board.sendMessage(token, PlayerMessage("You can't mortgage this field tile"), MessageType::INFO);
        }
        //get field tile
        //field tile ->
        return true;
    }
    if (reply->action == PlayerAction::START_TRADE) {
        //TODO:send request for number/token of player

        //TODO
        return true;
    }
    if(reply->action == PlayerAction::EXIT_GAME) {
        tile.board.decrNumOfOlayers();
        PlayerData& player = tile.board.getPlayer(token);
        player.setLoser();
        if (!tile.board.isFinished()) {
            tile.board.sendMessage(token, PlayerMessage("lose"), MessageType::INFO);
        } else {
            tile.board.sendMessage(token, PlayerMessage("win!"), MessageType::INFO);
        }
        return false;
    }
    if (reply->action == PlayerAction::BUY_BACK_PROPERTY) {
        //TODO:send request for number of field
        NumReply numReply = tile.board.sendNumRequest(token);
        std::cout << numReply->num << "\n";
        int index = numReply->num;
        if (tile.board.field[index]->isMortgaged && tile.board.field[index]->getOwner() == token) {
            PlayerData& player = tile.board.getPlayer(token);
            if ((double)player.getMoney() >= 1.1 * (double)tile.getMortgageCost()) {
                player.addMoney(-1.1 * (double)tile.getMortgageCost());
                player.numberOfMortgagedProperty--;
                tile.board.field[index]->isMortgaged = false;
            } else {
                tile.board.sendMessage(token, PlayerMessage("You don't have enough money :("), MessageType::INFO);
            }
        } else {
            tile.board.sendMessage(token, PlayerMessage("You can't buy back this field tile"), MessageType::INFO);
        }
        return true;
    }
    if (reply->action == PlayerAction::SELL_FIELD) {
        //TODO:send request for number of field
        NumReply numReply = tile.board.sendNumRequest(token);
        std::cout << numReply->num << "\n";
        int index = numReply->num;
        if (tile.board.field[index]->getNumberOfHouses() != 0 || tile.board.field[index]->getNumberOfHotels() != 0 ||
            tile.board.field[index]->getOwner() != token) {
            tile.board.sendMessage(token, PlayerMessage("You can't sell this field tile"), MessageType::INFO);
            return true;
        }
        //TODO checks
        PlayerData& player = tile.board.getPlayer(token);
        player.addMoney(tile.board.field[index]->getFieldCost());
        tile.board.field[index]->decrPropertyNum(token);
        tile.board.field[index]->setOwner(Token::FREE_FIELD);
    }
    if (reply->action == PlayerAction::SELL_HOUSE) {
        //TODO:send request for number of field
        NumReply numReply = tile.board.sendNumRequest(token);
        std::cout << numReply->num << "\n";
        int index = numReply->num;
        if (tile.board.field[index]->getNumberOfHouses() == 0 || tile.board.field[index]->getNumberOfHotels() != 0 ||
            tile.board.field[index]->getOwner() != token) {
            tile.board.sendMessage(token, PlayerMessage("You can't sell this building"), MessageType::INFO);
            return true;
        }
        //TODO checks
        PlayerData& player = tile.board.getPlayer(token);
        player.numberOfHouses--;
        player.addMoney(tile.board.field[index]->getHouseCost());
        tile.board.field[index]->decrHouseNum();
    }
    if (reply->action == PlayerAction::SELL_HOTEL) {
        //TODO:send request for number of field
        NumReply numReply = tile.board.sendNumRequest(token);
        std::cout << numReply->num << "\n";
        int index = numReply->num;
        if (tile.board.field[index]->getNumberOfHotels() == 0 || tile.board.field[index]->getOwner() != token) {
            tile.board.sendMessage(token, PlayerMessage("You can't sell this hotel"), MessageType::INFO);
            return true;
        }
        //TODO checks
        PlayerData& player = tile.board.getPlayer(token);
        player.numberOfHotels--;
        player.addMoney(tile.board.field[index]->getHotelCost());
        tile.board.field[index]->decrHotelNum();
    }
    return true;
}

void Start::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request;
    while (true) {
        makeDefaultRequest(request, token, board);
        if (player.numberOfMortgagedProperty != 0) {
            request.availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        PlayerReply reply = board.sendRequest(player.token, request);
        request.message = "";
        if (reply->action == PlayerAction::END_TURN) {
            break;
        }
        if (!handleGenericActions(token, *this, reply)) {
            return;
        }
    }
}

void Prison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    PlayerRequest request;
    bool diceUsed = false;
    while (true) {
        makeDefaultRequest(request, token, board);
        if (player.numberOfMortgagedProperty != 0) {
            request.availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        if (player.numberOfMortgagedProperty != 0) {
            request.availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        if (board.getPlayer(token).prisoner) {
            request.availableActions.push_back(PlayerAction::PAY_TAX);
            if(!diceUsed) {
                request.availableActions.push_back(PlayerAction::ROLL_DICE);
            }
            if (player.cardToLeavePrison) {
                request.availableActions.push_back(PlayerAction::ROLL_DICE);
            }
        }
        PlayerReply reply = board.sendRequest(token, request);
        request.message = "";
        if (reply->action == PlayerAction::END_TURN) {
            if (player.prisoner) {
                player.daysLeftInPrison--;
                if (player.daysLeftInPrison == 0) {
                    player.outOfPrison();
                }
            }
            break;
        }
        if (reply->action == PlayerAction::ROLL_DICE) {
            int firstThrow = rng.nextInt(1, 6), secondThrow = rng.nextInt(1, 6);
            if (firstThrow == secondThrow) {
                request.message = std::to_string(firstThrow) + " " + std::to_string(secondThrow) + "\nYou are leaving the prison!";
                player.outOfPrison();
            } else {
                request.message =
                        std::to_string(firstThrow) + " != " + std::to_string(secondThrow) + "\nYou don't leave prison :(";
            }
            diceUsed = true;
            continue;
        }
        if (reply->action == PlayerAction::PAY_TAX) {
            if (player.getMoney() >= tax) {
                player.addMoney(-tax);
                player.outOfPrison();
            } else {
                request.message = "Not enough money :(";
            }
            continue;
        }
        if (reply->action == PlayerAction::USE_CARD) {
            player.outOfPrison();
            player.cardToLeavePrison = false;
            continue;
        }
        if (!handleGenericActions(token, *this, reply)) {
            return;
        }
    }
}


void GoToPrison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    PlayerRequest request;
    player.toPrison();
    while (true) {
        request.availableActions = { PlayerAction::END_TURN }; //TODO: allocation each time. Rewrite with clear & push_back?
        if(player.prisoner) {
            request.availableActions.push_back(PlayerAction::ROLL_DICE);
        }
        PlayerReply reply = board.sendRequest(token, request);
        request.message = "";
        if (reply->action == PlayerAction::END_TURN) {
            break;
        }
        if (reply->action == PlayerAction::ROLL_DICE) {
            int firstThrow = rng.nextInt(1, 6), secondThrow = rng.nextInt(1, 6);
            if (firstThrow == secondThrow) {
                request.message = std::to_string(firstThrow) + " " + std::to_string(secondThrow) + "\nYou are leaving the prison!";
                player.outOfPrison();
            } else {
                request.message =
                        std::to_string(firstThrow) + " != " + std::to_string(secondThrow) + "\nYou don't leave prison :(";
            }
            continue;
        }
    }
}

void Chance::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    PlayerRequest request;
    //std::set<PlayerAction> mustHave = { PlayerAction::TAKE_CARD };
    bool takeCard = false;
    while (true) {
        makeDefaultRequest(request, token, board);
        if (player.numberOfMortgagedProperty != 0) {
            request.availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        //addAll(request.availableActions, mustHave);
        PlayerReply reply = board.sendRequest(token, request);
        request.message = "";
        if (reply->action == PlayerAction::END_TURN) {
            if (takeCard) {
                break;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::TAKE_CARD) {
            //mustHave.erase(PlayerAction::TAKE_CARD);
            takeCard = true;
            int firstThrow = rng.nextInt(1, 6), secondThrow = rng.nextInt(1, 6), thirdThrow = rng.nextInt(1, 6);
            std::size_t num = firstThrow + secondThrow + thirdThrow - 3;
       //     board.sendMessage(token, PlayerMessage(std::to_string(firstThrow) + " " + std::to_string(secondThrow)
         //                                               + " " + std::to_string(thirdThrow) + "\n" + cards[num]->text));
            board.sendMessage(token, PlayerMessage(cards[num]->text), MessageType::CHANCE);
            try {
                cards[num]->apply(player.token);
            } catch (...) {
                throw std::out_of_range("card num " + std::to_string(num));
            }
            continue;
        }
        if (!handleGenericActions(token, *this, reply)) {
            return;
        }
    }
}

void PublicTreasury::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    PlayerRequest request;
    std::set<PlayerAction> mustHave = { PlayerAction::TAKE_CARD };
    while (true) {
        makeDefaultRequest(request, token, board);
        if (player.numberOfMortgagedProperty != 0) {
            request.availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        addAll(request.availableActions, mustHave);
        PlayerReply reply = board.sendRequest(token, request);
        request.message = "";
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.empty()) {
                break;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::TAKE_CARD) {
            mustHave.erase(PlayerAction::TAKE_CARD);
            int firstThrow = rng.nextInt(1, 6), secondThrow = rng.nextInt(1, 6), thirdThrow = rng.nextInt(1, 6);
            std::size_t num = firstThrow + secondThrow + thirdThrow - 3;
//            board.sendMessage(token, PlayerMessage(std::to_string(firstThrow) + " " + std::to_string(secondThrow)
//                                                   + " " + std::to_string(thirdThrow) + "\n" + cards[num]->text));
            board.sendMessage(token, PlayerMessage(cards[num]->text), MessageType::PUBLIC_TREASURY);

            try {
                cards[num]->apply(player.token);
            } catch (...) {
                throw std::out_of_range("card num " + std::to_string(num));
            }
            continue;
        }
        if (!handleGenericActions(token, *this, reply)) {
            return;
        }
    }
}

void IncomeTax::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request;
    std::set<PlayerAction> mustHave = { PlayerAction::PAY_TAX };
    while (true) {
        makeDefaultRequest(request, token, board);
        if (player.numberOfMortgagedProperty != 0) {
            request.availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        addAll(request.availableActions, mustHave);
        PlayerReply reply = board.sendRequest(token, request);
        request.message = "";
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.empty()) {
                break;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::PAY_TAX) {
            if (player.getMoney() >= tax) {
                player.addMoney(-tax);
                mustHave.erase(PlayerAction::PAY_TAX);
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        if (!handleGenericActions(token, *this, reply)) {
            return;
        }
    }
}

std::set<PlayerAction> makePropertyMusthave(const OwnableTile& tile, Token token, bool taxPaid) {
    if (tile.owner == Token::FREE_FIELD) {
        return { PlayerAction::BUY_FIELD, PlayerAction::START_TRADE_NEW_FIELD };
    } else if (tile.owner != token && !taxPaid) {
        return { PlayerAction::PAY_TO_OTHER_PLAYER };
    } else {
        return {};
    }
}

void OwnableTile::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerData* fieldOwner = owner == Token::FREE_FIELD ? nullptr : &board.getPlayer(owner);
    PlayerRequest request;
    bool taxPaid = false;
    bool buyProperty = false;
    if (owner == token || isMortgaged || owner == Token::FREE_FIELD) {
        taxPaid = true;
    }
    if (owner != Token::FREE_FIELD && owner != token) {
        buyProperty = true;
    }
    while (true) {
        makeDefaultRequest(request, token, board);
        if (player.numberOfMortgagedProperty != 0) {
            request.availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        if (!buyProperty) {
            request.availableActions.push_back(PlayerAction::BUY_FIELD);
        }
        if (!taxPaid) {
            request.availableActions.push_back(PlayerAction::PAY_TO_OTHER_PLAYER);
        }
        PlayerReply reply = board.sendRequest(token, request);
        request.message = "";
        if (reply->action == PlayerAction::END_TURN) {
            std::cout << "PlayerAction::END_TURN\n";
            if (buyProperty && taxPaid) {
                break;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::PAY_TO_OTHER_PLAYER) {
            std::cout << "PlayerAction::PAY_TO_OTHER_PLAYER\n";
            if (taxPaid) {
                request.message = "You don't have to pay tax";
                continue;
            }
            tax = calculateTax(owner);
            if (player.getMoney() >= tax) {
                std::cout << "paying tax \n";
                player.addMoney(-tax);
                fieldOwner->addMoney(tax);
                taxPaid = true;
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        if (reply->action == PlayerAction::BUY_FIELD) {
            if (owner == token) {
                request.message = "You've already bought this field :)";
                continue;
            }
            if (owner != Token::FREE_FIELD) {
                request.message = "This field is not free";
                continue;
            }
            if (player.money >= cost) {
                player.money -= cost;
                onPurchase(token);
                buyProperty = true;
                owner = token;
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        if (reply->action == PlayerAction::START_TRADE_NEW_FIELD) {
            buyProperty = true;
        }
        if (!handleGenericActions(token, *this, reply)) {
            return;
        }
    }
}

size_t Railway::calculateTax(Token token) {
    if(owner == Token::FREE_FIELD) return 0;
    return 25 * (1u << board.getPlayer(owner).numberOfRailways);
}

void Railway::onPurchase(Token token) {
    board.getPlayer(token).numberOfRailways++;
}

size_t Utility::calculateTax(Token token) {
    if(owner == Token::FREE_FIELD) return 0;
    int lastThrow = board.getPlayer(token).lastTrow;
    switch(board.getPlayer(owner).numberOfUtilities) {
        case 1: return lastThrow * 4;
        case 2: return lastThrow * 10;
        default: assert(false);
    }
}

void Utility::onPurchase(Token token) {
    board.getPlayer(token).numberOfUtilities++;
}

std::vector<std::string> FieldTile::writeTileInfo() {
    std::vector<std::string> info;
    return info;
}

std::vector<std::string> OwnableTile::writeTileInfo() {
    std::vector<std::string> info;
    return info;
}

bool OwnableTile::MortgageTile(Token token) {
    if (this->board.getMortgageToken.count(owner) != 0 && owner == token) {
        isMortgaged = true;
        PlayerData& player = board.getPlayer(token);
        player.addMoney(mortgageCost);
        player.numberOfMortgagedProperty++;
        return true;
    }
    return false;
}

std::vector<std::string> Start::writeTileInfo() {
    std::vector<std::string> info = {"GO", "->"};
    return info;
}

std::vector<std::string> Street::writeTileInfo() {
    std::vector<std::string> info;
    info.push_back(name);
    std::string item = std::to_string(cost) + "$";
    info.push_back(item);
    item = "RENT: " + std::to_string(costOfParking);
    info.push_back(item);
    item = "OWNER: ";
    if (owner == Token::FREE_FIELD)
        item += "NONE";
    else if (owner == Token::SHIP)
        item += "SHIP";
    else if (owner == Token::BOOT)
        item += "BOOT";
    else if (owner == Token::CAR)
        item += "CAR";
    else if (owner == Token::CAT)
        item += "CAT";
    else if (owner == Token::DOG)
        item += "DOG";
    else if (owner == Token::HAT)
        item += "HAT";
    info.push_back(item);
    item = "______";
    info.push_back(item);
    item = "HOUSE PRICE: " + std::to_string(costPerHouse);
    info.push_back(item);
    //item = "HOTEL PRICE: " + std::to_string(costPerHotel);
    //info.push_back(item);
    return info;
}

void Street::setTaxes(int tax0, int tax1, int tax2, int tax3, int tax4, int tax5) {
    startTax = tax0;
    taxOneHouse = tax1;
    taxTwoHouses = tax3;
    taxThreeHouses = tax3;
    taxFourHouses = tax4;
    taxHotel = tax5;
}

std::vector<std::string> Railway::writeTileInfo() {
    std::vector<std::string> info;
    info.push_back(name);
    std::string item = std::to_string(cost) + "$";
    info.push_back(item);
    item = "RENT: " + std::to_string(costOfParking);
    info.push_back(item);
    item = "OWNER: ";
    if (owner == Token::FREE_FIELD)
        item += "NONE";
    else if (owner == Token::SHIP)
        item += "SHIP";
    else if (owner == Token::BOOT)
        item += "BOOT";
    else if (owner == Token::CAR)
        item += "CAR";
    else if (owner == Token::CAT)
        item += "CAT";
    else if (owner == Token::DOG)
        item += "DOG";
    else if (owner == Token::HAT)
        item += "HAT";
    info.push_back(item);
    item = "______";
    info.push_back(item);
    item = "if 2 railroads: ";
    info.push_back(item);
    item = "if 3 railroads: ";
    info.push_back(item);
    item = "if 4 railroads: ";
    info.push_back(item);
    item = "Moartage Value: ";
    info.push_back(item);
    return info;
}

void Railway::decrPropertyNum(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.numberOfRailways--;
}

std::vector<std::string> Utility::writeTileInfo() {
    std::vector<std::string> info;
    info.push_back(name);
    std::string item = "RENT: " + std::to_string(costOfParking);
    info.push_back(item);
    item = "Moartage Value: ";
    info.push_back(item);
    return info;
}

void Utility::decrPropertyNum(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.numberOfUtilities--;
}


std::vector<std::string> Prison::writeTileInfo() {
    std::vector<std::string> info;
    return info;
}

std::vector<std::string> GoToPrison::writeTileInfo() {
    std::vector<std::string> info;
    return info;
}

std::vector<std::string> Chance::writeTileInfo() {
    std::vector<std::string> info;
    return info;
}

std::vector<std::string> PublicTreasury::writeTileInfo() {
    std::vector<std::string> info;
    return info;
}

std::vector<std::string> IncomeTax::writeTileInfo() {
    std::vector<std::string> info;
    return info;
}

int IncomeTax::getTax() const {
    return tax;
}

std::vector<std::string> FreeParking::writeTileInfo() {
    std::vector<std::string> info;
    return info;
}