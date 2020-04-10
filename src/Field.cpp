#include "Field.h"
#include "Board.h"
#include "Random.h"
#include "Cards.h"

#include <utility>
#include <set>
#include <cassert>

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
    //TODO: fill vector cards
}

PublicTreasury::PublicTreasury(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {
    cards.resize(16);
    cards[0] = new LeftPrisonForFree(board);
    cards[1] = new PayMoney(board, "");//50
    cards[2] = new GetMoney(board, "Holiday pay! Get 100");//100
    cards[3] = new GetMoney(board, "");//100
    cards[4] = new PayMoney(board, "");//50
    cards[5] = new GetMoney(board, "");//20
    cards[6] = new GetMoney(board, "");//100
    cards[7] = new TeleportToPrison(board, "");
    cards[8] = new GetMoney(board, "");//25
    cards[9] = new GetMoneyFromOtherPlayers(board, "");//10
    cards[10] = new GetMoney(board, "");//100
    cards[11] = new GetMoney(board, "");//50
    cards[12] = new GetMoney(board, "");//10
    cards[13] = new PayMoney(board, "");//30*buildings + 115*hotels
    cards[14] = new Teleport(board, "");//to start
    cards[15] = new GetMoney(board, "");//200
    //TODO: fill vector cards
}

IncomeTax::IncomeTax(Board &board, int position, std::string name, int tax)
        : FieldTile(board, position, std::move(name)), tax(tax) {}

FreeParking::FreeParking(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {}

OwnableTile::OwnableTile(Board &board, int position, std::string name, int cost, Color color)
        : FieldTile(board, position, std::move(name)), cost(cost), color(color) {}

Railway::Railway(Board &board, int position, std::string name, int cost, Color color)
        : OwnableTile(board, position, std::move(name), cost, color) {}

Street::Street(Board &board, int position, std::string name, int cost, Color color, int costPerHouse)
        : OwnableTile(board, position, std::move(name), cost, color), costPerHouse(costPerHouse) {}

size_t Street::calculateTax(Token token) {
    // TODO
    return 0;
}

void Street::onPurchase(Token token) {
    // TODO
    return;
}

void Street::onPlayerEntry(Token token) {
    // TODO
    return;
}

Utility::Utility(Board &board, int position, std::string name, int cost, Color color)
        : OwnableTile(board, position, std::move(name), cost, color) {}

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

void makeDefaultRequest(PlayerRequest& r) {
    //TODO: New allocation each time... Maybe rewrite as clear, reserve & many push_backs?
    r.availableActions = {
            PlayerAction::END_TURN,
            PlayerAction::BUY_BUILDING,
            PlayerAction::BUY_HOTEL,
            PlayerAction::START_TRADE,
            PlayerAction::MORTGAGE_HOLDINGS
    };
}

void handleGenericActions(Token token, const PlayerReply& reply) {
    if (reply->action == PlayerAction::BUY_BUILDING) {
        //TODO
        return;
    }
    if (reply->action == PlayerAction::BUY_HOTEL) {
        //TODO
        return;
    }
    if (reply->action == PlayerAction::MORTGAGE_HOLDINGS) {
        //TODO
        return;
    }
    if (reply->action == PlayerAction::START_TRADE) {
        //TODO
        return;
    }
}

void Start::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request;
    while (true) {
        makeDefaultRequest(request);
        PlayerReply reply = board.sendRequest(player.token, request);
        if (reply->action == PlayerAction::END_TURN) {
            break;
        }
        handleGenericActions(token, reply);
    }
}

void Prison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    PlayerRequest request;
    bool diceUsed = false;
    while (true) {
        makeDefaultRequest(request);
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
            int firstTrow = rng.nextInt(1, 6), secondTrow = rng.nextInt(1, 6);
            if (firstTrow == secondTrow) {
                request.message = std::to_string(firstTrow) + " " + std::to_string(secondTrow) + "\nYou are leaving the prison!";
                player.outOfPrison();
            } else {
                request.message =
                        std::to_string(firstTrow) + " != " + std::to_string(secondTrow) + "\nYou don't leave prison :(";
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
        handleGenericActions(token, reply);
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
        if (reply->action == PlayerAction::END_TURN) {
            break;
        }
        if (reply->action == PlayerAction::ROLL_DICE) {
            int firstTrow = rng.nextInt(1, 6), secondTrow = rng.nextInt(1, 6);
            if (firstTrow == secondTrow) {
                request.message = std::to_string(firstTrow) + " " + std::to_string(secondTrow) + "\nYou are leaving the prison!";
                player.outOfPrison();
            } else {
                request.message =
                        std::to_string(firstTrow) + " != " + std::to_string(secondTrow) + "\nYou don't leave prison :(";
            }
            continue;
        }
    }
}

void Chance::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request;
    std::set<PlayerAction> mustHave = { PlayerAction::TAKE_CARD };
    while (true) {
        makeDefaultRequest(request);
        addAll(request.availableActions, mustHave);
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.empty()) {
                break;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::TAKE_CARD) {
            mustHave.erase(PlayerAction::TAKE_CARD);
            player.cards.push_back(board.deck.takeCard());
            continue;
        }
        handleGenericActions(token, reply);
    }
}

void PublicTreasury::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request;
    std::set<PlayerAction> mustHave = { PlayerAction::TAKE_CARD };
    while (true) {
        makeDefaultRequest(request);
        addAll(request.availableActions, mustHave);
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.empty()) {
                break;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::TAKE_CARD) {
            mustHave.erase(PlayerAction::TAKE_CARD);
            player.cards.push_back(board.deck.takeCard());
            continue;
        }
        handleGenericActions(token, reply);
    }
}

void IncomeTax::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request;
    std::set<PlayerAction> mustHave = { PlayerAction::PAY_TAX };

    while (true) {
        makeDefaultRequest(request);
        addAll(request.availableActions, mustHave);

        PlayerReply reply = board.sendRequest(token, request);
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
        handleGenericActions(token, reply);
    }
}

std::set<PlayerAction> makePropertyMusthave(const OwnableTile& tile, Token token, bool taxPaid) {
    if (tile.owner == Token::FREE_FIELD) {
        return { PlayerAction::BUY_PROPERTY, PlayerAction::START_TRADE_NEW_FIELD };
    } else if (tile.owner != token && !taxPaid) {
        return { PlayerAction::PAY_TO_OTHER_PLAYER };
    } else {
        return {};
    }
}

void OwnableTile::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerData& fieldOwner = board.getPlayer(owner);
    PlayerRequest request;
    bool taxPaid = false;

    while (true) {
        std::set<PlayerAction> mustHave = makePropertyMusthave(*this, token, taxPaid);
        makeDefaultRequest(request);
        addAll(request.availableActions, mustHave);

        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.empty()) {
                break;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::PAY_TO_OTHER_PLAYER) {
            int tax = calculateTax(token);
            if (player.getMoney() >= tax) {
                player.addMoney(-tax);
                fieldOwner.addMoney(tax);
                taxPaid = true;
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        if (reply->action == PlayerAction::BUY_PROPERTY) {
            if (player.money >= cost) {
                player.money -= cost;
                onPurchase(token);
                owner = token;
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        handleGenericActions(token, reply);
    }
}

size_t Railway::calculateTax(Token token) {
    return 25 * (1u << board.getPlayer(owner).numberOfRailways);
}

void Railway::onPurchase(Token token) {
    board.getPlayer(token).numberOfRailways++;
}

size_t Utility::calculateTax(Token token) {
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

std::string FieldTile::writeTileInfo() { return " "; }
std::string OwnableTile::writeTileInfo() { return " "; }
std::string Start::writeTileInfo() { return " "; }
std::string Street::writeTileInfo() { return " "; }
std::string Railway::writeTileInfo() { return " "; }
std::string Utility::writeTileInfo() { return " "; }
std::string Prison::writeTileInfo() { return " "; }
std::string GoToPrison::writeTileInfo() { return " "; }
std::string Chance::writeTileInfo() { return " "; }
std::string PublicTreasury::writeTileInfo() { return " "; }
std::string IncomeTax::writeTileInfo() { return " "; }
std::string FreeParking::writeTileInfo() { return " "; }