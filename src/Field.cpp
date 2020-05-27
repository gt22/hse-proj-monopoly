#include "Field.h"
#include "Board.h"
#include "Random.h"
#include "Cards.h"

#include <utility>
#include <set>
#include <cassert>
#include <iostream>
#include <PlayerRequests.h>

uint32_t Taxes::get(int id) const {
    switch (id) {
        case 0:
            return startTax;
        case 1:
            return taxOneHouse;
        case 2:
            return taxTwoHouses;
        case 3:
            return taxThreeHouses;
        case 4:
            return taxFourHouses;
        case 5:
            return taxHotel;
        default:
            assert(false);
    }
}

FieldTile::FieldTile(Board& board) : board(board) {}

FieldTile::FieldTile(Board& board, TileType type, int position, std::string name) :
        board(board), type(type), position(position), name(std::move(name)) {}

Start::Start(Board& board) : FieldTile(board) {}

Start::Start(Board& board, int position, std::string name)
        : FieldTile(board, TileType::START, position, std::move(name)) {}

Prison::Prison(Board& board) : FieldTile(board) {}

Prison::Prison(Board& board, int position, std::string name)
        : FieldTile(board, TileType::PRISON, position, std::move(name)) {}

GoToPrison::GoToPrison(Board& board) : FieldTile(board) {}

GoToPrison::GoToPrison(Board& board, int position, std::string name)
        : FieldTile(board, TileType::GOTO_PRISON, position, std::move(name)) {}

Chance::Chance(Board& board) : FieldTile(board) {}

Chance::Chance(Board& board, int position, std::string name)
        : FieldTile(board, TileType::CHANCE, position, std::move(name)) {
    cards.resize(16);
    cards[0] = new Teleport(board, "Visit Arbat street");
    cards[0]->setPos(ARBAT_POS);

    cards[1] = new Teleport(board, "Take a ride on Riga railway.\n Get M200 when passing field \"Start\"");
    cards[1]->setPos(RIGA_RAILWAY_POS);

    cards[2] = new GetMoney(board, "Loan for building. Get M150");
    cards[2]->setAmount(150);

    cards[3] = new Teleport(board, "Visit Mayakovsky square.\n Get M200 when passing field \"Start\"");
    cards[3]->setPos(MAYAKOVSKY_SQUARE);

    //TODO: cards[4]
    cards[4] = new Teleport(board, "Visit Mayakovsky square.\n Get M200 when passing field \"Start\"");
    cards[4]->setPos(MAYAKOVSKY_SQUARE);
    //TODO: cards[4]

    cards[5] = new TeleportToPrison(board,
                                    "You've been arrested. Go straight to the prison.\n You won't get M200 when passing field \"Start\"");

    cards[6] = new Teleport(board, "Go to the field \"Start\" and get M200");
    cards[6]->setPos(START_POS);

    cards[7] = new GetMoney(board, "Bank pays you dividends in amount of M50");
    cards[7]->setAmount(50);

    cards[8] = new PayMoney(board,
                            "It's the capital repair time.\n Pay M25 for each house/building, M100 for each hotel you have.");
    cards[8]->setFlag(true);

    cards[9] = new LeavePrisonForFree(board, "Get out of jail for free");

    cards[10] = new Teleport(board, "Visit Polyanka street.\n Get M200 when passing field \"Start\"");
    cards[10]->setPos(POLYANKA_POS);

    //TODO: fill vector cards!!!
    cards[11] = new GetMoney(board, "From sale of stock you earn M50");//50
    cards[11]->setAmount(50);

    cards[12] = new GetMoney(board, "You won second place in a beauty contest. Collect M10");//10
    cards[12]->setAmount(10);

    cards[13] = new PayMoney(board,
                             "You are to make streets repair.\n Pay M40 for each house and M115 for each hotel in your property.");//30*buildings + 115*hotels
    cards[13]->setFlag(true);

    cards[14] = new Teleport(board, "Pass \"Go\". Collect M200");//to start
    cards[14]->setPos(START_POS);

    cards[15] = new GetMoney(board, "Bank error in your favor. Collect M200");//200
    cards[15]->setAmount(200);
    //TODO: fill vector cards!!!
}


PublicTreasury::PublicTreasury(Board& board) : FieldTile(board) {}

PublicTreasury::PublicTreasury(Board& board, int position, std::string name)
        : FieldTile(board, TileType::PUBLIC_TREASURY, position, std::move(name)) {
    cards.resize(16);
    cards[0] = new LeavePrisonForFree(board, "Get out of jail for free!");

    cards[1] = new PayMoney(board, "Expenses for treatment at the doctor. Pay M50");//50
    cards[1]->setAmount(50);

    cards[2] = new GetMoney(board, "Vacation pay. Collect M100");//100
    cards[2]->setAmount(100);

    cards[3] = new GetMoney(board, "Payment for insurance. Collect M100");//100
    cards[3]->setAmount(100);

    cards[4] = new PayMoney(board, " Educational expenses. Pay M50");//50
    cards[4]->setAmount(50);

    cards[5] = new GetMoney(board, "Income tax refund. Collect M20");//20
    cards[5]->setAmount(20);

    cards[6] = new GetMoney(board, "Pay hospital. Collect M100");//100
    cards[6]->setAmount(100);

    cards[7] = new TeleportToPrison(board,
                                    "You're under arrest! Move directly to jail.\n Don't pass \"Go\". Don't collect M200");

    cards[8] = new GetMoney(board, "Get M25 for the consultation");//25
    cards[8]->setAmount(25);

    cards[9] = new GetMoneyFromOtherPlayers(board, "Birthday! Get M10 from each of participants");//10
    cards[9]->setAmount(10);

    cards[10] = new GetMoney(board, "You get an inheritance M100");//100
    cards[10]->setAmount(100);

    cards[11] = new GetMoney(board, "From sale of stock you earn M50");//50
    cards[11]->setAmount(50);

    cards[12] = new GetMoney(board, "You won second place in a beauty contest. Collect M10");//10
    cards[12]->setAmount(10);

    cards[13] = new PayMoney(board,
                             "You are to make streets repair.\n Pay M40 for each house and M115 for each hotel in your property.");//30*buildings + 115*hotels
    cards[13]->setFlag(true);

    cards[14] = new Teleport(board, "Pass \"Go\". Collect M200");//to start
    cards[14]->setPos(START_POS);

    cards[15] = new GetMoney(board, "Bank error in your favor. Collect M200");//200
    cards[15]->setAmount(200);
    //TODO: fill vector cards
}

IncomeTax::IncomeTax(Board& board) : FieldTile(board) {}

IncomeTax::IncomeTax(Board& board, int position, std::string name, uint32_t tax)
        : FieldTile(board, TileType::INCOME_TAX, position, std::move(name)), tax(tax) {}

FreeParking::FreeParking(Board& board) : FieldTile(board) {}

FreeParking::FreeParking(Board& board, int position, std::string name)
        : FieldTile(board, TileType::FREE_PARKING, position, std::move(name)) {}

OwnableTile::OwnableTile(Board& board) : FieldTile(board) {}

OwnableTile::OwnableTile(Board& board, int position, std::string name, std::string shortName, OwnableType ownableType,
                         uint32_t cost,
                         Color color)
        : FieldTile(board, TileType::OWNABLE_TILE, position, std::move(name)), shortName(std::move(shortName)),
          ownableType(ownableType), cost(cost),
          color(color) {}


Railway::Railway(Board& board) : OwnableTile(board) { mortgageCost = 100; }

Railway::Railway(Board& board, int position, std::string name, std::string shortName, int cost, Color color)
        : OwnableTile(board, position, std::move(name), std::move(shortName), OwnableType::RAILWAY, cost,
                      color) { mortgageCost = 100; }

Street::Street(Board& board) : OwnableTile(board) {}

Street::Street(Board& board, int position, std::string name, std::string shortName, int cost, Color color,
               int costPerHouse, int costPerHotel)
        : OwnableTile(board, position, std::move(name), std::move(shortName), OwnableType::STREET, cost, color),
          costPerHouse(costPerHouse), costPerHotel(costPerHotel) {}

Utility::Utility(Board& board) : OwnableTile(board) { mortgageCost = 75; }

Utility::Utility(Board& board, int position, std::string name, std::string shortName, int cost, Color color)
        : OwnableTile(board, position, std::move(name), std::move(shortName), OwnableType::UTILITY, cost,
                      color) { mortgageCost = 75; }


uint32_t Street::calculateTax(Token token) {
    return costOfParking = taxes.get(numberOfHouses + numberOfHotels);
}

void Street::onPurchase(Token token) {
    // TODO
    return;
}

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

PlayerRequest makeDefaultRequest(Token token, Board& board) {
    PlayerRequest r = Monopoly::Requests::action(token,
                                                 {PlayerAction::EXIT_GAME, PlayerAction::FINISH_GAME});
    const PlayerData& player = board.getPlayer(token);
    if (player.numberOfHotels > 0) {
        r->availableActions.push_back(PlayerAction::SELL_HOTEL);
    }
    if (board.atLeastOneBoughtFieldOtherToken(token)) {
        r->availableActions.push_back(PlayerAction::START_TRADE);
    }
    if (player.numberOfHouses > 0) {
        //TODO: CHECK HOTELS
        r->availableActions.push_back(PlayerAction::SELL_HOUSE);
    }
    if (board.checkFieldWithoutBuildings(token)) {
        r->availableActions.push_back(PlayerAction::SELL_FIELD);
        r->availableActions.push_back(PlayerAction::MORTGAGE_HOLDINGS);
    }
    if (board.checkAllFieldsOfCol(token)) {
        //TODO: check
        r->availableActions.push_back(PlayerAction::BUY_HOUSE);
        r->availableActions.push_back(PlayerAction::BUY_HOTEL);
    }
    return r;
}

bool handleGenericActions(Token tok, const FieldTile& tile, PlayerAction action) {
    using namespace Monopoly::Requests;
    if (action == PlayerAction::LOSE) {
        PlayerData& player = tile.board.getPlayer(tok);
        player.setLoser();
        tile.board.makePlayerLoser(player.token);
        tile.board.decrNumOfOlayers();
        tile.board.send(message(tok, MessageType::INFO, "lose"));
        //TODO
        return false;
    }
    if (action == PlayerAction::BUY_HOUSE) {
        PlayerReply numReply = tile.board.send(num(tok));
        size_t index = numReply->data.num;
        auto chosenField = tile.board.getFieldTile(index);
        if (tok != chosenField->getOwner() || chosenField->getColor() == Color::NO_COL || chosenField->isMortgaged ||
            chosenField->getNumberOfHouses() >= 4 || chosenField->getNumberOfHotels() > 0) {
            tile.board.send(message(tok, MessageType::INFO, "You can't build house on this field tile"));
            return true;
        }
        if (!tile.board.checkAllFieldsOfCurColor(tok, index)) {
            tile.board.send(message(tok, MessageType::INFO, "You can't build house on this field tile"));
            return true;
        }
        if (!checkPrevForHouse(index, tile.board) || !checkNextForBuyHouse(index, tile.board)) {
            tile.board.send(message(tok, MessageType::INFO, "You can't build house on this field tile"));

            return true;
        }
        PlayerData& player = chosenField->board.getPlayer(tok);
        if (player.getMoney() >= chosenField->getHouseCost()) {
            player.addMoney(-chosenField->getHouseCost());
            player.numberOfHouses++;
            chosenField->addHouse();
        } else {
            tile.board.send(message(tok, MessageType::INFO, "You don't have enough money :("));
        }
        return true;
    }
    if (action == PlayerAction::BUY_HOTEL) {
        PlayerReply numReply = tile.board.send(num(tok));
        size_t index = numReply->data.num;
        auto chosenField = tile.board.getFieldTile(index);
        if (tok != chosenField->getOwner() || chosenField->getColor() == Color::NO_COL || chosenField->isMortgaged ||
            chosenField->getNumberOfHouses() < 4 || chosenField->getNumberOfHotels() > 0) {
            tile.board.send(message(tok, MessageType::INFO, "You can't build hotel on this field tile"));
            return true;
        }
        if (!tile.board.checkAllFieldsOfCurColor(tok, index)) {
            tile.board.send(message(tok, MessageType::INFO, "You can't build hotel on this field tile"));
            return true;
        }
        if (!checkPrevForHotel(index, tile.board) || !checkNextForBuyHotel(index, tile.board)) {
            tile.board.send(message(tok, MessageType::INFO, "You can't build hotel on this field tile"));
            return true;
        }
        PlayerData& player = tile.board.getPlayer(tok);
        if (player.getMoney() >= chosenField->getHotelCost()) {
            player.addMoney(-chosenField->getHotelCost());
            player.numberOfHotels++;
            chosenField->addHotel();
        } else {
            tile.board.send(message(tok, MessageType::INFO, "You don't have enough money :("));
        }
        return true;
    }
    if (action == PlayerAction::MORTGAGE_HOLDINGS) {
        PlayerReply numReply = tile.board.send(num(tok));
        int index = numReply->data.num;
        auto chosenField = tile.board.getFieldTile(index);
        if (chosenField->getNumberOfHouses() != 0 && chosenField->getNumberOfHotels() != 0) {
            tile.board.send(message(tok, MessageType::INFO, "You can't mortgage this field tile"));
            return true;
        }
        if (!chosenField->isMortgaged && chosenField->getOwner() == tok) {
            PlayerData& player = tile.board.getPlayer(tok);
            player.addMoney(chosenField->getMortgageCost());
            player.numberOfMortgagedProperty++;
            chosenField->isMortgaged = true;
        } else {
            tile.board.send(message(tok, MessageType::INFO, "You can't mortgage this field tile"));
        }
        return true;
    }
    if (action == PlayerAction::START_TRADE) {
        PlayerReply tokenReply = tile.board.send(token(tok));
        while (tokenReply->data.token == tok) {
            tokenReply = tile.board.send(token(tok));
        }
        PlayerReply numReply = tile.board.send(num(tok));
        auto chosenField = tile.board.getFieldTile(numReply->data.num);
        if (!chosenField->canBeSold(tokenReply->data.token)) {
            tile.board.send(message(tok, MessageType::INFO, "You can't buy this field tile from " +
                                                            tile.board.tokenToString(tokenReply->data.token)));
            return true;
        }
        PlayerReply sumReply = tile.board.send(sum(tok));
        tile.board.send(message(tokenReply->data.token, MessageType::INFO,
                                "Field for sale: " + tile.name + "\nCurrent cost: "
                                + std::to_string(sumReply->data.sum) +
                                "\nCurrent buyer: " +
                                tile.board.tokenToString(tok)));

        PlayerReply tradeReply = tile.board.send(trade(tok));
        if (tradeReply->data.trade == PlayerTradeAction::PARTICIPATE) {
            PlayerData& player = tile.board.getPlayer(tok);
            if (player.getMoney() >= sumReply->data.sum) {
                PlayerData& fieldOwner = tile.board.getPlayer(tokenReply->data.token);
                player.addMoney(-sumReply->data.sum);
                fieldOwner.addMoney(sumReply->data.sum);
                chosenField->onPurchase(player.token);
                chosenField->decrPropertyNum(fieldOwner.token);
                chosenField->setOwner(player.token);
            } else {
                tile.board.send(message(player.token, MessageType::INFO, "Not enough money :("));
                //player.setLoser();
            }
        } else {
            tile.board.send(message(tok, MessageType::INFO,
                                    tile.board.tokenToString(tok) + " doesn't want to sell\n" + tile.name));
        }
        return true;
    }
    if (action == PlayerAction::EXIT_GAME) {
        tile.board.decrNumOfOlayers();
        PlayerData& player = tile.board.getPlayer(tok);
        player.setLoser();
        tile.board.makePlayerLoser(player.token);
        tile.board.send(message(tok, MessageType::INFO,
                                tile.board.isFinished() ? "win!" : "lose"));
        return false;
    }
    if (action == PlayerAction::BUY_BACK_PROPERTY) {
        PlayerReply numReply = tile.board.send(num(tok));
        int index = numReply->data.num;
        auto& selectedTile = *tile.board.getFieldTile(index);
        if (selectedTile.isMortgaged && selectedTile.getOwner() == tok) {
            PlayerData& player = tile.board.getPlayer(tok);
            if ((double) player.getMoney() >= 1.1 * (double) tile.getMortgageCost()) {
                player.addMoney(-1.1 * (double) tile.getMortgageCost());
                player.numberOfMortgagedProperty--;
                selectedTile.isMortgaged = false;
            } else {
                tile.board.send(message(tok, MessageType::INFO, "You don't have enough money :("));
            }
        } else {
            tile.board.send(message(tok, MessageType::INFO, "You can't buy back this field tile"));
        }
        return true;
    }
    if (action == PlayerAction::SELL_FIELD) {
        PlayerReply numReply = tile.board.send(num(tok));
        auto& selectedTile = *tile.board.getFieldTile(numReply->data.num);
        if (selectedTile.getNumberOfHouses() != 0 ||
            selectedTile.getNumberOfHotels() != 0 ||
            selectedTile.getOwner() != tok ||
            selectedTile.isMortgaged) {
            tile.board.send(message(tok, MessageType::INFO, "You can't sell this field tile"));
            return true;
        }
        if (!checkToSellField(numReply->data.num, tile.board)) {
            tile.board.send(message(tok, MessageType::INFO, "You can't sell this field tile"));
            return true;
        }
        //TODO checks
        PlayerData& player = tile.board.getPlayer(tok);
        player.addMoney(selectedTile.getFieldCost());
        selectedTile.decrPropertyNum(tok);
        selectedTile.setOwner(Token::FREE_FIELD);
    }

    if (action == PlayerAction::SELL_HOUSE) {
        PlayerReply numReply = tile.board.send(num(tok));
        int index = numReply->data.num;
        auto& selectedTile = *tile.board.getFieldTile(index);
        if (selectedTile.getNumberOfHouses() == 0 ||
            selectedTile.getNumberOfHotels() != 0 ||
            selectedTile.getOwner() != tok) {
            tile.board.send(message(tok, MessageType::INFO, "You can't sell building on this field"));
            return true;
        }
        if (!checkNextForHouse(index, tile.board)) {
            tile.board.send(message(tok, MessageType::INFO, "You can't sell this building"));
            return true;
        }
        //TODO checks
        PlayerData& player = tile.board.getPlayer(tok);
        player.numberOfHouses--;
        player.addMoney(selectedTile.getHouseCost());
        selectedTile.decrHouseNum();
    }
    if (action == PlayerAction::SELL_HOTEL) {
        PlayerReply numReply = tile.board.send(num(tok));
        int index = numReply->data.num;
        auto& selectedTile = *tile.board.getFieldTile(index);
        if (selectedTile.getNumberOfHotels() == 0 || selectedTile.getOwner() != tok) {
            tile.board.send(message(tok, MessageType::INFO, "You can't sell hotel on this field"));
            return true;
        }
        if (!checkNextForHotel(index, tile.board)) {
            tile.board.send(message(tok, MessageType::INFO, "You can't sell this hotel"));
            return true;
        }
        //TODO checks
        PlayerData& player = tile.board.getPlayer(tok);
        player.numberOfHotels--;
        player.addMoney(selectedTile.getHotelCost());
        selectedTile.decrHotelNum();
    }
    return true;
}

void Start::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request = makeDefaultRequest(token, board);
    while (true) {
        request->availableActions.push_back(PlayerAction::END_TURN);
        if (player.numberOfMortgagedProperty != 0) {
            request->availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        PlayerAction action = board.send(std::move(request))->data.action;
        request = makeDefaultRequest(token, board);
        if (action == PlayerAction::END_TURN) {
            break;
        }
        if (!handleGenericActions(token, *this, action)) {
            return;
        }
    }
}

const int Prison::TAX = 50;

void Prison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    bool diceUsed = false;
    PlayerRequest request = makeDefaultRequest(token, board);
    while (true) {
        request->availableActions.push_back(PlayerAction::END_TURN);
        if (player.numberOfMortgagedProperty != 0) {
            request->availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        if (board.getPlayer(token).prisoner) {
            request->availableActions.push_back(PlayerAction::PAY_TAX);
            if (!diceUsed) {
                request->availableActions.push_back(PlayerAction::ROLL_DICE);
            }
            if (player.cardToLeavePrison) {
                request->availableActions.push_back(PlayerAction::ROLL_DICE);
            }
        }
        PlayerAction action = board.send(std::move(request))->data.action;
        request = makeDefaultRequest(token, board);
        if (action == PlayerAction::END_TURN) {
            if (player.prisoner) {
                player.daysLeftInPrison--;
                if (player.daysLeftInPrison == 0) {
                    player.outOfPrison();
                }
            }
            break;
        }
        if (action == PlayerAction::ROLL_DICE) {
            int firstThrow = rng.nextInt(1, 6), secondThrow = rng.nextInt(1, 6);
            if (firstThrow == secondThrow) {
                request->message.message = std::to_string(firstThrow) + " " + std::to_string(secondThrow) +
                                   "\nYou are leaving the prison!";
                player.outOfPrison();
            } else {
                request->message.message =
                        std::to_string(firstThrow) + " != " + std::to_string(secondThrow) +
                        "\nYou don't leave prison :(";
            }
            diceUsed = true;
            continue;
        }
        if (action == PlayerAction::PAY_TAX) {
            if (player.money >= TAX) {
                player.money -= TAX;
                player.outOfPrison();
            } else {
                request->message.message = "Not enough money :(";
            }
            continue;
        }
        if (action == PlayerAction::USE_CARD) {
            player.outOfPrison();
            player.cardToLeavePrison = false;
            continue;
        }
        if (!handleGenericActions(token, *this, action)) {
            return;
        }
    }

}


void GoToPrison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    player.toPrison();
    PlayerRequest request = Monopoly::Requests::action(token, {PlayerAction::END_TURN});
    while (true) {
        if (player.prisoner) {
            request->availableActions.push_back(PlayerAction::ROLL_DICE);
        }
        PlayerAction action = board.send(std::move(request))->data.action;
        request = Monopoly::Requests::action(token, {PlayerAction::END_TURN});
        if (action == PlayerAction::END_TURN) {
            break;
        }
        if (action == PlayerAction::ROLL_DICE) {
            int firstThrow = rng.nextInt(1, 6), secondThrow = rng.nextInt(1, 6);
            if (firstThrow == secondThrow) {
                request->message.message = std::to_string(firstThrow) + " " + std::to_string(secondThrow) +
                                   "\nYou are leaving the prison!";
                player.outOfPrison();
            } else {
                request->message.message =
                        std::to_string(firstThrow) + " != " + std::to_string(secondThrow) +
                        "\nYou don't leave prison :(";
            }
            continue;
        }
    }
}

void Chance::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    //std::set<PlayerAction> mustHave = { PlayerAction::TAKE_CARD };
    bool tookCard = false;
    PlayerRequest request = makeDefaultRequest(token, board);
    while (true) {
        request->availableActions.push_back(tookCard ? PlayerAction::END_TURN : PlayerAction::TAKE_CARD);
        if (player.numberOfMortgagedProperty != 0) {
            request->availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        //addAll(request.availableActions, mustHave);
        PlayerAction action = board.send(std::move(request))->data.action;
        request = makeDefaultRequest(token, board);
        if (action == PlayerAction::END_TURN) {
            if (tookCard) {
                break;
            }
            request->message.message = "You can't finish turn";
            continue;
        }
        if (action == PlayerAction::TAKE_CARD) {
            tookCard = true;
            int firstThrow = rng.nextInt(1, 6), secondThrow = rng.nextInt(1, 6), thirdThrow = rng.nextInt(1, 6);
            std::size_t num = firstThrow + secondThrow + thirdThrow - 3;
            board.send(Monopoly::Requests::message(token, MessageType::CHANCE, cards[num]->text));
            try {
                cards[num]->apply(player.token);
            } catch (...) {
                throw std::out_of_range("card num " + std::to_string(num));
            }
            continue;
        }
        if (!handleGenericActions(token, *this, action)) {
            return;
        }
    }
}

void PublicTreasury::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    PlayerRequest request = makeDefaultRequest(token, board);
    std::set<PlayerAction> mustHave = {PlayerAction::TAKE_CARD};
    while (true) {
        if (mustHave.empty()) {
            request->availableActions.push_back(PlayerAction::END_TURN);
        }
        if (player.numberOfMortgagedProperty != 0) {
            request->availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        addAll(request->availableActions, mustHave);
        PlayerAction action = board.send(std::move(request))->data.action;
        request = makeDefaultRequest(token, board);
        if (action == PlayerAction::END_TURN) {
            if (mustHave.empty()) {
                break;
            }
            request->message.message = "You can't finish turn";
            continue;
        }
        if (action == PlayerAction::TAKE_CARD) {
            mustHave.erase(PlayerAction::TAKE_CARD);
            int firstThrow = rng.nextInt(1, 6), secondThrow = rng.nextInt(1, 6), thirdThrow = rng.nextInt(1, 6);
            std::size_t num = firstThrow + secondThrow + thirdThrow - 3;
            board.send(Monopoly::Requests::message(token, MessageType::PUBLIC_TREASURY, cards[num]->text));

            if (num >= cards.size()) throw std::out_of_range("card num " + std::to_string(num));
            cards[num]->apply(player.token);
            continue;
        }
        if (!handleGenericActions(token, *this, action)) {
            return;
        }
    }
}

void IncomeTax::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request = makeDefaultRequest(token, board);
    std::set<PlayerAction> mustHave = {PlayerAction::PAY_TAX};
    while (true) {
        if (mustHave.empty()) {
            request->availableActions.push_back(PlayerAction::END_TURN);
        }
        if (player.numberOfMortgagedProperty != 0) {
            request->availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        addAll(request->availableActions, mustHave);
        PlayerAction action = board.send(std::move(request))->data.action;
        request = makeDefaultRequest(token, board);
        if (action == PlayerAction::END_TURN) {
            if (mustHave.empty()) {
                break;
            }
            request->message.message = "You can't finish turn";
            continue;
        }
        if (action == PlayerAction::PAY_TAX) {
            if (player.getMoney() >= tax) {
                player.addMoney(-tax);
                mustHave.erase(PlayerAction::PAY_TAX);
                continue;
            }
            request->message.message = "Not enough money :(";
            continue;
        }
        if (!handleGenericActions(token, *this, action)) {
            return;
        }
    }
}

std::set<PlayerAction> makePropertyMusthave(const OwnableTile& tile, Token token, bool taxPaid) {
    if (tile.owner == Token::FREE_FIELD) {
        return {PlayerAction::BUY_FIELD, PlayerAction::START_TRADE_NEW_FIELD};
    } else if (tile.owner != token && !taxPaid) {
        return {PlayerAction::PAY_TO_OTHER_PLAYER};
    } else {
        return {};
    }
}

void OwnableTile::onPlayerEntry(Token token) {
    using namespace Monopoly::Requests;
    PlayerData& player = board.getPlayer(token);
    PlayerData *fieldOwner = owner == Token::FREE_FIELD ? nullptr : &board.getPlayer(owner);
    bool taxPaid = false;
    bool buyProperty = false;
    if (owner == token || isMortgaged || owner == Token::FREE_FIELD) {
        taxPaid = true;
    }
    if (owner != Token::FREE_FIELD) {
        buyProperty = true;
    }
    PlayerRequest request = makeDefaultRequest(token, board);
    while (true) {
        board.setPlayerIndex(board.getPlayerNum(token));
        if (buyProperty && taxPaid) {
            request->availableActions.push_back(PlayerAction::END_TURN);
        }
        if (player.numberOfMortgagedProperty != 0) {
            request->availableActions.push_back(PlayerAction::BUY_BACK_PROPERTY);
        }
        if (!buyProperty) {
            request->availableActions.push_back(PlayerAction::BUY_FIELD);
            request->availableActions.push_back(PlayerAction::START_TRADE_NEW_FIELD);
        }
        if (!taxPaid) {
            request->availableActions.push_back(PlayerAction::PAY_TO_OTHER_PLAYER);
        }

        PlayerAction action = board.send(std::move(request))->data.action;
        request = makeDefaultRequest(token, board);
        if (action == PlayerAction::END_TURN) {
            if (buyProperty && taxPaid) {
                break;
            }
            request->message.message = "You can't finish turn";
            continue;
        }
        costOfParking = calculateTax(owner);
        if (action == PlayerAction::PAY_TO_OTHER_PLAYER) {
            if (taxPaid) {
                request->message.message = "You don't have to pay tax";
                continue;
            }
            uint32_t tax = calculateTax(token);
            if (player.getMoney() >= tax) {
                player.addMoney(-tax);
                fieldOwner->addMoney(tax);
                taxPaid = true;
                continue;
            }
            request->message.message = "Not enough money :(";
            continue;
        }
        if (action == PlayerAction::BUY_FIELD) {
            if (owner == token) {
                request->message.message = "You've already bought this field :)";
                continue;
            }
            if (owner != Token::FREE_FIELD) {
                request->message.message = "This field is not free";
                continue;
            }
            if (player.money >= cost) {
                player.money -= cost;
                onPurchase(token);
                buyProperty = true;
                owner = token;
                costOfParking = calculateTax(token);
                continue;
            }
            request->message.message = "Not enough money :(";
            continue;
        }
        if (action == PlayerAction::START_TRADE_NEW_FIELD) {
            //TODO
            int numOfParticipants = board.getCurNumOfPlayers() - 1;
            std::vector<bool> participants(board.getPlayersNumber(), true);
            int curPlayerNum = board.getPlayerNum(token);
            board.setPlayerIndex(curPlayerNum);
            participants[curPlayerNum] = false;
            int curCost = cost;
            int curBuyer = -1;
            while (numOfParticipants > 1) {
                if (!participants[curPlayerNum]) {
                    curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
                    board.setPlayerIndex(curPlayerNum);
                    continue;
                }
                PlayerData& curPlayer = board.getPlayer(board.getPlayerToken(curPlayerNum));
                if (!curPlayer.alive) {
                    curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
                    board.setPlayerIndex(curPlayerNum);
                    continue;
                }
                if (curCost > curPlayer.getMoney()) {
                    participants[curPlayerNum] = false;
                    numOfParticipants--;
                    curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
                    board.setPlayerIndex(curPlayerNum);
                    continue;
                }
                board.send(message(curPlayer.token, MessageType::INFO,
                                   "Auction field: " + this->name + "\nCurrent cost: "
                                   + std::to_string(curCost) + "\nCurrent buyer: " +
                                   board.numToTokenString(curBuyer)));
                PlayerTradeAction tradeReply = board.send(trade(curPlayer.token))->data.trade;
                if (tradeReply == PlayerTradeAction::REFUSE) {
                    participants[curPlayerNum] = false;
                    numOfParticipants--;
                }
                if (tradeReply == PlayerTradeAction::PARTICIPATE) {
                    int sumReply = board.send(sum(curPlayer.token))->data.sum;
                    if (curPlayer.getMoney() <= sumReply) {
                        board.send(message(curPlayer.token, MessageType::INFO, "You don't have enough money"));
                    } else if (curCost < sumReply) {
                        curBuyer = curPlayerNum;
                        curCost = sumReply;
                    }
                }
                curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
                board.setPlayerIndex(curPlayerNum);
            }
            while (true) {
                PlayerData& curPlayer = board.getPlayer(board.getPlayerToken(curPlayerNum));
                if (!curPlayer.alive || !participants[curPlayerNum]) {
                    curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
                    board.setPlayerIndex(curPlayerNum);
                } else {
                    break;
                }
            }
            PlayerData& curPlayer = board.getPlayer(board.getPlayerToken(curPlayerNum));
            if (curBuyer == curPlayerNum) {
                if (curPlayer.money >= curCost) {
                    curPlayer.money -= curCost;
                    onPurchase(curPlayer.token);
                    buyProperty = true;
                    owner = curPlayer.token;
                    costOfParking = calculateTax(curPlayer.token);
                    continue;
                } else {
                    board.send(message(curPlayer.token, MessageType::INFO, "Not enough money :("));
                    curPlayer.setLoser();
                    board.makePlayerLoser(curPlayer.token);
                }
            } else {
                board.send(message(curPlayer.token, MessageType::INFO,
                                   "Auction field: " + this->name + "\nCurrent cost: "
                                   + std::to_string(curCost) + "\nCurrent buyer: " +
                                   board.numToTokenString(curBuyer)));
                PlayerTradeAction tradeReply = board.send(trade(curPlayer.token))->data.trade;
                if (tradeReply == PlayerTradeAction::PARTICIPATE) {
                    if (curPlayer.money >= cost) {
                        curPlayer.money -= cost;
                        onPurchase(curPlayer.token);
                        buyProperty = true;
                        owner = curPlayer.token;
                        costOfParking = calculateTax(curPlayer.token);
                        continue;
                    } else {
                        board.send(message(curPlayer.token, MessageType::INFO, "Not enough money :("));
                        curPlayer.setLoser();
                        board.makePlayerLoser(curPlayer.token);
                    }
                }
            }
            buyProperty = true;
            board.setPlayerIndex(board.getPlayerNum(token));
        }
        if (!handleGenericActions(token, *this, action)) {
            return;
        }
    }
}

uint32_t Railway::calculateTax(Token token) {
    if (owner == Token::FREE_FIELD)
        costOfParking = 0;
    else
        costOfParking = 25 * (1u << board.getPlayer(owner).numberOfRailways);
    return costOfParking;
}

void Railway::onPurchase(Token token) {
    board.getPlayer(token).numberOfRailways++;
}

uint32_t Utility::calculateTax(Token token) {
    if (owner == Token::FREE_FIELD) return 0;
    int lastThrow = board.getPlayer(token).lastTrow;
    switch (board.getPlayer(owner).numberOfUtilities) {
        case 1:
            costOfParking = lastThrow * 4;
            return lastThrow * 4;
        case 2:
            costOfParking = lastThrow * 10;
            return lastThrow * 10;
        default:
            assert(false);
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

bool OwnableTile::canBeSold(Token token) const {
    return (owner == token) && (this->getNumberOfHouses() == 0) &&
           (this->getNumberOfHotels() == 0) && (!isMortgaged);
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

void Street::setTaxes(Taxes t) {
    taxes = t;
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