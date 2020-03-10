#ifndef BOARD_H
#define BOARD_H

#include "MonopolyFwd.h"
#include "Cards.h"
#include "PlayerRequests.h"
#include <string_view>
#include <array>
#include <vector>

static constexpr int PRISON_POZ = 29;

struct PlayerData {
    PlayerData(std::string_view name, Token token);
    std::string_view name;
    Token token;
    std::size_t position = 0;
    int money = 0;
    int doubleDice = 0;
    int daysLeftInPrison = 0; //TODO: придумать нормальное имя
    bool prisoner = false;
    std::vector<std::unique_ptr<Card>> cards;

    void newPosition(std::size_t throwSum);
    void toPrison();
};

class Board {
public:
    Board(const std::vector<std::pair<std::string_view, Token>>& players, Game& game);
    static constexpr int FIELD_SIZE = 40;
    std::array<FieldTile *, FIELD_SIZE>  field;
    CardPool deck;

    PlayerData& getPlayer(Token);
    Token getPlayerToken(std::size_t index);
    const std::vector<PlayerData>& getPlayers();
    PlayerReply sendRequest(Token token, PlayerRequest request);
    std::size_t getPlayersNumber();

private:
    std::vector<PlayerData> players;
    Game& game;
};

#endif //BOARD_H