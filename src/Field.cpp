#include "Field.h"
#include "Board.h"
#include "Random.h"

#include <utility>
#include <set>

FieldTile::FieldTile(Board& board, int position, std::string name) :
                        board(board), position(position), name(std::move(name)) {}

void FieldTile::onPlayerPass(Token) {}

void FieldTile::onPlayerEntry(Token) {}

void Start::onPlayerPass(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.money += START_SUM;
}

template<typename T>
int findInVector(std::vector<T>& vec, T val) {
    for (std::size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == val) {
            return i;
        }
    }
    return -1;
}

Start::Start(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}

void Start::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    player.money += START_SUM;
    PlayerRequest request({}, "");
    std::set<PlayerAction> mustHave;
    mustHave.insert(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::USE_CARD);
    request.availableActions.push_back(PlayerAction::BUY_BUILDING);
    request.availableActions.push_back(PlayerAction::BUY_HOTEL);
    request.availableActions.push_back(PlayerAction::MORTGAGE_HOLDINGS);
    request.availableActions.push_back(PlayerAction::START_TRADE);
    while (!mustHave.empty()) {
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            mustHave.clear();
            continue;
        }
        if (reply->action == PlayerAction::USE_CARD) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_BUILDING) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_HOTEL) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::MORTGAGE_HOLDINGS) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::START_TRADE) {
            //TODO
            continue;
        }
    }
}

Prison::Prison(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {}

void Prison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    PlayerRequest request({}, "");
    std::set<PlayerAction> mustHave;
    mustHave.insert(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::BUY_BUILDING);
    request.availableActions.push_back(PlayerAction::BUY_HOTEL);
    request.availableActions.push_back(PlayerAction::START_TRADE);
    request.availableActions.push_back(PlayerAction::MORTGAGE_HOLDINGS);
    if (board.getPlayer(token).prisoner) {
        request.availableActions.push_back(PlayerAction::PAY_TAX);
        request.availableActions.push_back(PlayerAction::USE_CARD);
        request.availableActions.push_back(PlayerAction::ROLL_DICE);
    }
    while (!mustHave.empty()) {
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            if (player.prisoner) {
                player.daysLeftInPrison--;
                if (player.daysLeftInPrison == 0) {
                    player.outOfPrison();
                }
            }
            mustHave.clear();
            continue;
        }
        if (reply->action == PlayerAction::ROLL_DICE) {
            int firstTrow = rng.nextInt(1, 6), secondTrow = rng.nextInt(1, 6);
            if (firstTrow == secondTrow) {
                request.message = std::to_string(firstTrow) + " " + std::to_string(secondTrow) + "\nYou are leaving the prison!";
                player.outOfPrison();
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::USE_CARD));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::PAY_TAX));
            }
            request.message = std::to_string(firstTrow) + " != " + std::to_string(secondTrow) + "\nYou don't leave prison :(";
            request.availableActions.erase(request.availableActions.begin() +
                                           findInVector(request.availableActions, PlayerAction::ROLL_DICE));
            continue;
        }
        if (reply->action == PlayerAction::PAY_TAX) {
            if (player.money >= tax) {
                player.money -= tax;
                player.outOfPrison();
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::USE_CARD));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::PAY_TAX));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::ROLL_DICE));
            } else {
                request.message = "Not enough money :(";
            }
            continue;
        }
        if (reply->action == PlayerAction::USE_CARD) {
            //TODO: пользователь видит список доступных карт, выбирает из них
            continue;
        }
        if (reply->action == PlayerAction::BUY_BUILDING) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_HOTEL) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::START_TRADE) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::MORTGAGE_HOLDINGS) {
            //TODO
            continue;
        }
    }
}

GoToPrison::GoToPrison(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}


void GoToPrison::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    RandomSource rng;
    PlayerRequest request({}, "");
    std::set<PlayerAction> mustHave;
    mustHave.insert(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::ROLL_DICE);
    player.toPrison();
    while (!mustHave.empty()) {
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            mustHave.clear();
            continue;
        }
        if (reply->action == PlayerAction::ROLL_DICE) {
            int firstTrow = rng.nextInt(1, 6), secondTrow = rng.nextInt(1, 6);
            if (firstTrow == secondTrow) {
                request.message = std::to_string(firstTrow) + " " + std::to_string(secondTrow) + "\nYou are leaving the prison!";
                player.outOfPrison();
            }
            request.message = std::to_string(firstTrow) + " != " + std::to_string(secondTrow) + "\nYou don't leave prison :(";
            request.availableActions.erase(request.availableActions.begin() +
                                           findInVector(request.availableActions, PlayerAction::ROLL_DICE));
            continue;
        }
    }
}

Chance::Chance(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}

void Chance::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request({}, "");
    std::set<PlayerAction> mustHave;
    mustHave.insert(PlayerAction::END_TURN);
    mustHave.insert(PlayerAction::TAKE_CARD);
    request.availableActions.push_back(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::USE_CARD);
    request.availableActions.push_back(PlayerAction::START_TRADE);
    request.availableActions.push_back(PlayerAction::MORTGAGE_HOLDINGS);
    request.availableActions.push_back(PlayerAction::BUY_HOTEL);
    request.availableActions.push_back(PlayerAction::BUY_BUILDING);
    request.availableActions.push_back(PlayerAction::TAKE_CARD);
    while (!mustHave.empty()) {
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.find(PlayerAction::TAKE_CARD) == mustHave.end()) {
                mustHave.clear();
                continue;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::TAKE_CARD) {
            mustHave.erase(mustHave.find(PlayerAction::TAKE_CARD));
            request.availableActions.erase(request.availableActions.begin() +
                                           findInVector(request.availableActions, PlayerAction::TAKE_CARD));
            player.cards.push_back(board.deck.takeCard());
            continue;
        }
        if (reply->action == PlayerAction::USE_CARD) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::START_TRADE) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::MORTGAGE_HOLDINGS) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_BUILDING) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_HOTEL) {
            //TODO
            continue;
        }
    }
}

IncomeTax::IncomeTax(Board &board, int position, std::string name, int tax)
        : FieldTile(board, position, std::move(name)), tax(tax) {}

void IncomeTax::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerRequest request({}, "");
    std::set<PlayerAction> mustHave;
    mustHave.insert(PlayerAction::END_TURN);
    mustHave.insert(PlayerAction::PAY_TAX);
    request.availableActions.push_back(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::USE_CARD);
    request.availableActions.push_back(PlayerAction::START_TRADE);
    request.availableActions.push_back(PlayerAction::MORTGAGE_HOLDINGS);
    request.availableActions.push_back(PlayerAction::BUY_HOTEL);
    request.availableActions.push_back(PlayerAction::BUY_BUILDING);
    request.availableActions.push_back(PlayerAction::PAY_TAX);
    while (!mustHave.empty()) {
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.find(PlayerAction::PAY_TAX) == mustHave.end()) {
                mustHave.clear();
                continue;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::PAY_TAX) {
            if (player.money >= tax) {
                player.money -= tax;
                mustHave.erase(mustHave.find(PlayerAction::PAY_TAX));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::PAY_TAX));
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        if (reply->action == PlayerAction::USE_CARD) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::START_TRADE) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::MORTGAGE_HOLDINGS) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_BUILDING) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_HOTEL) {
            //TODO
            continue;
        }
    }
}

FreeParking::FreeParking(Board &board, int position, std::string name)
        : FieldTile(board, position, std::move(name)) {}

void FreeParking::onPlayerEntry(Token token) { }

int getRailwayTax(int num) {
    int tax = 25;
    for (int i = 1; i <= num; i++) {
        tax *= 2;
    }
    return tax;
}

Railway::Railway(Board &board, int position, std::string name, int cost, Color color)
        : OwnableTile(board, position, std::move(name), cost, color) {}

void Railway::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerData& fieldOwner = board.getPlayer(owner);
    PlayerRequest request({}, "");
    std::set<PlayerAction> mustHave;
    mustHave.insert(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::BUY_HOTEL);
    request.availableActions.push_back(PlayerAction::USE_CARD);
    request.availableActions.push_back(PlayerAction::BUY_BUILDING);
    request.availableActions.push_back(PlayerAction::START_TRADE);
    request.availableActions.push_back(PlayerAction::MORTGAGE_HOLDINGS);
    if (owner == Token::FREE_FIELD) {
        request.availableActions.push_back(PlayerAction::BUY_PROPERTY);
        mustHave.insert(PlayerAction::BUY_PROPERTY);
        request.availableActions.push_back(PlayerAction::START_TRADE_NEW_FIELD);
        mustHave.insert(PlayerAction::START_TRADE_NEW_FIELD);
    } else if (owner != token) {
        request.availableActions.push_back(PlayerAction::PAY_TO_OTHER_PLAYER);
        mustHave.insert(PlayerAction::PAY_TO_OTHER_PLAYER);
    }
    while (!mustHave.empty()) {
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.size() == 1) {
                mustHave.clear();
                continue;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::PAY_TO_OTHER_PLAYER) {
            int tax = getRailwayTax(fieldOwner.numberOfRailways);
            if (player.money >= tax) {
                player.money -= tax;
                fieldOwner.money += tax;
                mustHave.erase(mustHave.find(PlayerAction::PAY_TO_OTHER_PLAYER));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::PAY_TO_OTHER_PLAYER));
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        if (reply->action == PlayerAction::BUY_PROPERTY) {
            if (player.money >= cost) {
                player.money -= cost;
                player.numberOfRailways++;
                owner = token;
                mustHave.erase(mustHave.find(PlayerAction::BUY_PROPERTY));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::BUY_PROPERTY));
                mustHave.erase(mustHave.find(PlayerAction::START_TRADE_NEW_FIELD));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::START_TRADE_NEW_FIELD));
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
    }
}

OwnableTile::OwnableTile(Board &board, int position, std::string name, int cost, Color color)
 : FieldTile(board, position, std::move(name)), cost(cost), color(color) {}

Street::Street(Board &board, int position, std::string name, int cost, Color color, int costPerHouse)
 : OwnableTile(board, position, std::move(name), cost, color), costPerHouse(costPerHouse) {}

void Street::onPlayerEntry(Token token) {}

Utility::Utility(Board &board, int position, std::string name, int cost, Color color)
 : OwnableTile(board, position, std::move(name), cost, color) {}

int getUtilityTax(int sum, int numberOfUtility) {
    if (numberOfUtility == 1) {
        return sum * 4;
    }
    if (numberOfUtility == 2) {
        return sum * 10;
    }
    return -1;
}

void Utility::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    PlayerData& fieldOwner = board.getPlayer(owner);
    PlayerRequest request({}, "");
    std::set<PlayerAction> mustHave;
    mustHave.insert(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::END_TURN);
    request.availableActions.push_back(PlayerAction::BUY_HOTEL);
    request.availableActions.push_back(PlayerAction::USE_CARD);
    request.availableActions.push_back(PlayerAction::BUY_BUILDING);
    request.availableActions.push_back(PlayerAction::START_TRADE);
    request.availableActions.push_back(PlayerAction::MORTGAGE_HOLDINGS);
    if (owner == Token::FREE_FIELD) {
        request.availableActions.push_back(PlayerAction::BUY_PROPERTY);
        mustHave.insert(PlayerAction::BUY_PROPERTY);
        request.availableActions.push_back(PlayerAction::START_TRADE_NEW_FIELD);
        mustHave.insert(PlayerAction::START_TRADE_NEW_FIELD);
    } else if (owner != token) {
        request.availableActions.push_back(PlayerAction::PAY_TO_OTHER_PLAYER);
        mustHave.insert(PlayerAction::PAY_TO_OTHER_PLAYER);
    }
    while (!mustHave.empty()) {
        PlayerReply reply = board.sendRequest(token, request);
        if (reply->action == PlayerAction::END_TURN) {
            if (mustHave.size() == 1) {
                mustHave.clear();
                continue;
            }
            request.message = "You can't finish turn";
            continue;
        }
        if (reply->action == PlayerAction::PAY_TO_OTHER_PLAYER) {
            int tax = getUtilityTax(player.lastTrow, fieldOwner.numberOfUtilities);
            if (player.money >= tax) {
                player.money -= tax;
                fieldOwner.money += tax;
                mustHave.erase(mustHave.find(PlayerAction::PAY_TO_OTHER_PLAYER));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::PAY_TO_OTHER_PLAYER));
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        if (reply->action == PlayerAction::BUY_PROPERTY) {
            if (player.money >= cost) {
                player.money -= cost;
                player.numberOfUtilities++;
                owner = token;
                mustHave.erase(mustHave.find(PlayerAction::BUY_PROPERTY));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::BUY_PROPERTY));
                mustHave.erase(mustHave.find(PlayerAction::START_TRADE_NEW_FIELD));
                request.availableActions.erase(request.availableActions.begin() +
                                               findInVector(request.availableActions, PlayerAction::START_TRADE_NEW_FIELD));
                continue;
            }
            request.message = "Not enough money :(";
            continue;
        }
        if (reply->action == PlayerAction::USE_CARD) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::START_TRADE) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::MORTGAGE_HOLDINGS) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_BUILDING) {
            //TODO
            continue;
        }
        if (reply->action == PlayerAction::BUY_HOTEL) {
            //TODO
            continue;
        }
    }
}