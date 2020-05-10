#include "view/BoardModel.h"
#include "Field.h"
void PlayerModel::update(const PlayerData& player) {
    name = player.name;
    token = player.token;
    position = player.position;
    money = player.money;
    prisoner = player.prisoner;
    alive = player.alive;
}

void FieldTileModel::update(const FieldTile& tile) {
    position = tile.position;
    name = tile.name;
    if(auto ownable = dynamic_cast<const OwnableTile*>(&tile); ownable) {
        cost = ownable->cost;
        costOfParking = ownable->costOfParking;
        owner = ownable->owner;
        if(auto street = dynamic_cast<const Street*>(ownable); street) {
            numberOfHouses = street->numberOfHouses;
            costPerHouse = street->costPerHouse;
            color = street->color;
            taxes = street->taxes;
        }
    } else if(auto prison = dynamic_cast<const Prison*>(&tile); prison) {
        tax = Prison::TAX;
    } else if(auto incometax = dynamic_cast<const IncomeTax*>(&tile); incometax) {
        tax = incometax->tax;
    }
}

void BoardModel::update(const Board& board) {
    for (size_t i = 0; i < FIELD_SIZE; ++i) {
        field[i].update(*board.getField()[i]);
    }
    players.resize(board.getPlayers().size());
    for (size_t i = 0; i < players.size(); ++i) {
        players[i].update(board.getPlayers()[i]);
    }
    curPlayer = board.getPlayerToken(board.getPlayerIndex());
}
