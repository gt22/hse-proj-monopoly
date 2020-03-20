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
    board.getPlayer(token).cards.push_back(board.deck.takeCard());
}


void IncomeTax::onPlayerEntry(Token token) {
    PlayerData& player = board.getPlayer(token);
    /*//check property
    if (player.money < tax) {
        // aks other players if they want to buy smth(???)
    }
    while (player.money < tax) {
        if (!board.sendRequest(token, PlayerRequest("You don't have enough money.\n Would you like to mortgage real estate?")).answer) {
            break;
        }
        //what do you want to mortgage?
    }
    if (player.money >= tax) {
        player.money -= tax;
        return;
    }
    //lose*/
}

IncomeTax::IncomeTax(Board &board, int position, std::string name, int tax)
 : FieldTile(board, position, std::move(name)), tax(tax) {}

void FreeParking::onPlayerPass(Token token) { static_cast<void>(token); }

FreeParking::FreeParking(Board &board, int position, std::string name)
 : FieldTile(board, position, std::move(name)) {}

void Railway::onPlayerEntry(Token token) {
    /*if (owner == token) {
        return;
    }
    PlayerData& player = board.getPlayer(token);
    if (owner == Token::FREE_FIELD) {
        if (board.sendRequest(token, PlayerRequest("Would you like to buy" + this->name + "?")).answer) {
            if (player.money >= cost) {
                player.money -= cost;
                (*this).owner = player.token;
                return;
            }
            //sendMessage "You don't have enough money"
        }
        //TODO: покупка торги
        return;
    }
    PlayerData& fieldOwner = board.getPlayer(owner);
    int tax = 25;
    for (int i = 1; i <= fieldOwner.numberOfRailways; i++) {
        tax *= 2;
    }
    static_cast<void>(player);
    //TODO: проверить, что может заплатить*/
}

OwnableTile::OwnableTile(Board &board, int position, std::string name, int cost, Color color)
 : FieldTile(board, position, std::move(name)), cost(cost), color(color) {}

Railway::Railway(Board &board, int position, std::string name, int cost, Color color)
 : OwnableTile(board, position, std::move(name), cost, color) {}

Street::Street(Board &board, int position, std::string name, int cost, Color color, int costPerHouse)
 : OwnableTile(board, position, std::move(name), cost, color), costPerHouse(costPerHouse) {}

Utility::Utility(Board &board, int position, std::string name, int cost, Color color)
 : OwnableTile(board, position, std::move(name), cost, color) {}
