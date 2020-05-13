#ifndef BOARDMODEL_H
#define BOARDMODEL_H
#include <string_view>
#include <optional>
#include "MonopolyFwd.h"
#include "Board.h"

struct PlayerModel {
    std::string_view name;
    Token token;
    std::size_t position;
    int money;
    bool prisoner;
    bool alive;

    void update(const PlayerData& player);
};

struct FieldTileModel {
    int position;
    std::string_view name;
   // std::optional<int> tax;
    std::optional<int> cost;
    std::optional<int> costOfParking;
    std::optional<Color> color;
    std::optional<Token> owner;
    std::optional<int> numberOfHouses;
    std::optional<int> costPerHouse;
    bool isStreet = false;
    void update(const FieldTile& tile);
};

struct BoardModel {
    constexpr static size_t FIELD_SIZE = Board::FIELD_SIZE;
    std::array<FieldTileModel, FIELD_SIZE> field;
    std::vector<PlayerModel> players;
    Token curPlayer;

    void update(const Board& board);

};

#endif //BOARDMODEL_H