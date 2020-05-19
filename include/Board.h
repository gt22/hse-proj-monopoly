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
static constexpr int POLYANKA_POS = 11;
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
    int numberOfMortgagedProperty = 0;
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
    static constexpr size_t FIELD_SIZE = 40;
    std::array<FieldTile *, FIELD_SIZE>  field;
    CardPool deck;

    void sync();

    virtual PlayerData& getPlayer(Token token);
    const PlayerData& getPlayer(Token token) const;
    FieldTile *getFieldTile(int ind);
    Token getPlayerToken(std::size_t index) const;
    std::size_t getPlayerNum(Token token) const;
    const std::vector<PlayerData>& getPlayers() const;
    virtual PlayerReply sendRequest(Token token, PlayerRequest request) const;
    virtual NumReply sendNumRequest(Token token) const;
    virtual SumReply sendSumRequest(Token token) const;
    virtual TokenReply sendTokenRequest(Token token) const;
    virtual PlayerTradeReply sendTradeRequest(Token token, PlayerTradeRequest request) const;
    void sendMessage(Token token, PlayerMessage mes, MessageType type) const;
    std::size_t getPlayersNumber() const;
    void decrNumOfOlayers();
    bool isFinished() const;
    void terminate();
    int getCurNumOfPlayers() const;
    Token getWinner() const;
    FieldTile* getTile(std::size_t pos) const;
    bool checkAllFieldsOfCurColor(Token token, int ind) const;
    bool checkFieldWithoutBuildings(Token token) const;
    bool checkAllFieldsOfCol(Token token) const;

    std::string numToTokenString(int ind) const;
    std::string tokenToString(Token token) const;

    bool atLeastOneBoughtFieldOtherToken(Token token) const;

    std::size_t getPlayerIndex() const;
    void setPlayerIndex(std::size_t num);
    std::map<Token, TokenForMortgage> getMortgageToken;

private:
    std::vector<PlayerData> players;
    Game& game;
    int numOfAlivePlayers = 0;
    bool terminated = false;

    std::size_t index = 0;
};

bool checkPrevForHotel(int ind, const Board& board);
bool checkPrevForHouse(int ind, const Board& board);

bool checkNextForHotel(int ind, const Board& board);
bool checkNextForHouse(int ind, const Board& board);

int countPrevForColor(int ind, const Board& board);
bool checkHousesNumForHotel(int ind, const Board& board);
bool checkHousesNumForHouse(int ind, const Board& board);
bool checkPrevHotel(int ind, const Board& board);
bool checkPrevHouse(int ind, const Board& board);

#endif //BOARD_H
