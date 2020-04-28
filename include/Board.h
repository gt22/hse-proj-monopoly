#ifndef BOARD_H
#define BOARD_H

#include "MonopolyFwd.h"
#include "Cards.h"
#include "PlayerRequests.h"
#include <string_view>
#include <array>
#include <vector>

static constexpr int START_POS = 0;
static constexpr int RIGA_RAILWAY_POS = 5;
static constexpr int PRISON_POS = 10;
static constexpr int MAYAKOVSKY_SQUARE = 24;
static constexpr int ARBAT_POS = 39;

struct PlayerData {
    PlayerData(std::string_view name, Token token);
    PlayerData(const PlayerData&) = delete;
    virtual ~PlayerData() = default;
    void operator=(PlayerData) = delete;
    PlayerData(PlayerData&&) noexcept = default;
    PlayerData& operator=(PlayerData&&) noexcept = default;
    std::string_view name;
    Token token;
    std::size_t position = 0;
    int money = 1500;
    int doubleDice = 0;
    int daysLeftInPrison = 0; //TODO: придумать нормальное имя
    size_t numberOfRailways = 0;
    size_t numberOfUtilities = 0;

    int numberOfHouses = 0;
    int numberOfHotels = 0;
    int lastTrow = 0;
    bool prisoner = false;
    bool alive = true;
    bool cardToLeavePrison = false;
    std::vector<std::unique_ptr<Card>> cards;

    int getMoney();
    int addMoney(int newMoney);
    void setLoser();

    void newPosition(int throwSum);
    void toPrison();
    void outOfPrison();
};

class Board {
public:
    Board(const std::vector<std::pair<std::string_view, Token>>& players, Game& game);
    static constexpr int FIELD_SIZE = 40;
    std::array<FieldTile *, FIELD_SIZE>  field;
    CardPool deck;

    void sync();

    virtual PlayerData& getPlayer(Token token);
    const PlayerData& getPlayer(Token token) const;
    Token getPlayerToken(std::size_t index) const;
    const std::vector<PlayerData>& getPlayers() const;
    virtual PlayerReply sendRequest(Token token, PlayerRequest request) const;
    void sendMessage(Token token, PlayerMessage mes) const;
    std::size_t getPlayersNumber() const;
    void decrNumOfOlayers();
    bool isFinished() const;
    int getCurNumOfPlayers() const;
    Token getWinner() const;
    FieldTile* getTile(std::size_t pos) const;


private:
    std::vector<PlayerData> players;
    Game& game;
    int numOfAlivePlayers;
};

#endif //BOARD_H
