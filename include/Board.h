#ifndef BOARD_H
#define BOARD_H

#include "MonopolyFwd.h"
#include "Cards.h"
#include "PlayerRequests.h"
#include <string_view>
#include <array>
#include <vector>

static constexpr int PRISON_POS = 10;

struct PlayerData {
    PlayerData(std::string name, Token token);
    PlayerData(const PlayerData&) = delete;
    virtual ~PlayerData() = default;
    void operator=(PlayerData) = delete;
    PlayerData(PlayerData&&) noexcept = default;
    PlayerData& operator=(PlayerData&&) noexcept = default;
    std::string name;
    Token token;
    uint32_t position = 0;
    uint32_t money = 1500; //TODO: Negative money?
    uint32_t doubleDice = 0;
    uint32_t daysLeftInPrison = 0;
    uint32_t numberOfRailways = 0;
    uint32_t numberOfUtilities = 0;
    uint32_t lastTrow = 0;
    bool prisoner = false;
    bool alive = true;
    std::vector<std::unique_ptr<Card>> cards;

    void newPosition(std::size_t throwSum);
    void toPrison();
    void outOfPrison();
};

class Board {
public:
    explicit Board(Game& game); // Creates uninitialized board, for deserialization only
    Board(const std::vector<std::pair<std::string_view, Token>>& players, Game& game);
    static constexpr int FIELD_SIZE = 40;


    virtual PlayerData& getPlayer(Token token);
    const PlayerData& getPlayer(Token token) const;
    Token getPlayerToken(std::size_t index) const;
    virtual PlayerReply sendRequest(Token token, PlayerRequest request) const;
    void sendMessage(Token token, PlayerMessage mes) const;
    std::size_t getPlayersNumber() const;

    std::array<FieldTile*, FIELD_SIZE>& getField();
    const std::array<FieldTile*, FIELD_SIZE>& getField() const;

    std::vector<PlayerData>& getPlayers();
    const std::vector<PlayerData>& getPlayers() const;

    CardPool& getDeck();
    const CardPool& getDeck() const;

private:
    Game& game;
    std::array<FieldTile*, FIELD_SIZE> field; //TODO: raw pointer?
    std::vector<PlayerData> players;
    CardPool deck;
};

#endif //BOARD_H
