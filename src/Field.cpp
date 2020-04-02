#include "Field.h"
#include "Board.h"
#include "Random.h"

#include <utility>
#include <set>
#include <cassert>

FieldTile::FieldTile(Board &board) : board(board) {}
FieldTile::FieldTile(Board& board, TileType type, int position, std::string name) :
                        board(board), type(type), position(position), name(std::move(name))  {}

Start::Start(Board &board) : FieldTile(board) {}
Start::Start(Board &board, int position, std::string name)
        : FieldTile(board, TileType::START, position, std::move(name)) {}

Prison::Prison(Board &board) : FieldTile(board) {}
Prison::Prison(Board &board, int position, std::string name)
        : FieldTile(board, TileType::PRISON, position, std::move(name)) {}

GoToPrison::GoToPrison(Board &board) : FieldTile(board) {}
GoToPrison::GoToPrison(Board &board, int position, std::string name)
        : FieldTile(board, TileType::GOTO_PRISON, position, std::move(name)) {}

Chance::Chance(Board &board) : FieldTile(board) {}
Chance::Chance(Board &board, int position, std::string name)
        : FieldTile(board, TileType::CHANCE, position, std::move(name)) {}

IncomeTax::IncomeTax(Board &board) : FieldTile(board) {}
IncomeTax::IncomeTax(Board &board, int position, std::string name, uint32_t tax)
        : FieldTile(board, TileType::INCOME_TAX, position, std::move(name)), tax(tax) {}

FreeParking::FreeParking(Board &board) : FieldTile(board) {}
FreeParking::FreeParking(Board &board, int position, std::string name)
        : FieldTile(board, TileType::FREE_PARKING, position, std::move(name)) {}

OwnableTile::OwnableTile(Board &board) : FieldTile(board) {}
OwnableTile::OwnableTile(Board &board, int position, std::string name, OwnableType ownableType, uint32_t cost, Color color)
        : FieldTile(board, TileType::OWNABLE_TILE, position, std::move(name)), ownableType(ownableType), cost(cost), color(color)  {}

Railway::Railway(Board &board) : OwnableTile(board) {}
Railway::Railway(Board &board, int position, std::string name, int cost, Color color)
        : OwnableTile(board, position, std::move(name), OwnableType::RAILWAY, cost, color) {}

Street::Street(Board &board) : OwnableTile(board) {}
Street::Street(Board &board, int position, std::string name, int cost, Color color, int costPerHouse)
        : OwnableTile(board, position, std::move(name), OwnableType::STREET, cost, color), costPerHouse(costPerHouse) {}

Utility::Utility(Board &board) : OwnableTile(board) {}
Utility::Utility(Board &board, int position, std::string name, int cost, Color color)
        : OwnableTile(board, position, std::move(name), OwnableType::UTILITY, cost, color) {}

uint32_t Street::calculateTax(Token token) {
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

void FieldTile::onPlayerPass(Token) {}

void FieldTile::onPlayerEntry(Token) {

    //TODO: generic actions?

}

void Start::onPlayerPass(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.money += START_SUM;
}

template<typename T, typename C>
void addAll(std::vector<T>& vec, const C& cont) {
    std::copy(cont.begin(), cont.end(), std::back_inserter(vec));
}

void makeDefaultRequest(PlayerRequest& r) {
    //TODO: New allocation each time... Maybe rewrite as clear, reserve & many push_backs?
    r.availableActions = {
            PlayerAction::END_TURN,
            PlayerAction::USE_CARD,
            PlayerAction::BUY_BUILDING,
            PlayerAction::BUY_HOTEL,
            PlayerAction::START_TRADE,
            PlayerAction::MORTGAGE_HOLDINGS //TODO: вроде оно не добавлялось в список доступных действий, но при этом обрабатывалось обычно...
    };
}

void handleGenericActions(Token token, const PlayerReply& reply) {
    if (reply->action == PlayerAction::USE_CARD) {
        //TODO
        return;
    }
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
    player.money += START_SUM;
    PlayerRequest request;
    while (true) {
        makeDefaultRequest(request);
        PlayerReply reply = board.sendRequest(token, request);
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
            if (player.money >= TAX) {
                player.money -= TAX;
                player.outOfPrison();
            } else {
                request.message = "Not enough money :(";
            }
            continue;
        }
        //TODO: нужна-ли здесь специальная обработка, или сойдёт запихнуть её в общий случай?
        if (reply->action == PlayerAction::USE_CARD) {
            //TODO: пользователь видит список доступных карт, выбирает из них
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
            if (player.money >= tax) {
                player.money -= tax;
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
            uint32_t tax = calculateTax(token);
            if (player.money >= tax) {
                player.money -= tax;
                fieldOwner.money += tax;
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

uint32_t Railway::calculateTax(Token token) {
    return 25 * (1u << board.getPlayer(owner).numberOfRailways);
}

void Railway::onPurchase(Token token) {
    board.getPlayer(token).numberOfRailways++;
}



uint32_t Utility::calculateTax(Token token) {
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