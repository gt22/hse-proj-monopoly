#ifndef PLAYERREQUESTS_H
#define PLAYERREQUESTS_H

#include <vector>
#include <string>
#include <memory>

enum class PlayerAction : std::size_t {
    END_TURN = 0,
    PAY_TAX = 1,
    BUY_PROPERTY = 2,
    BUY_BUILDING = 3,
    BUY_HOTEL = 4,
    USE_CARD = 5,
    PAY_TO_OTHER_PLAYER = 6,
    TAKE_CARD = 7,
    MORTGAGE_HOLDINGS = 8,
    START_TRADE = 9,
    START_TRADE_NEW_FIELD = 10,
    ROLL_DICE = 11,
    BUY_BACK_PROPERTY = 12,
    EXIT_GAME = 13,
    LOSE = 14
};

struct PlayerRequest final {
    PlayerRequest();
    explicit PlayerRequest(std::vector<PlayerAction> availableActions,
            std::string message);
    std::vector<PlayerAction> availableActions;
    std::string message;
};

struct PlayerMessage final {
    explicit PlayerMessage(std::string message);
    std::string message;
};

struct PlayerReplyData {
    explicit PlayerReplyData(PlayerAction action);
    virtual ~PlayerReplyData() = default;
    const PlayerAction action;
};

template<PlayerAction ACTION>
struct BasicReply : PlayerReplyData {
    BasicReply() : PlayerReplyData(ACTION) {}
};

struct RollDiceReply : BasicReply<PlayerAction::ROLL_DICE> {};

struct PayTaxReply : BasicReply<PlayerAction::PAY_TAX> {};

struct BuyPropertyReply : BasicReply<PlayerAction::BUY_PROPERTY> {};

struct BuyBuildingReply : BasicReply<PlayerAction::BUY_BUILDING> {};

struct EndTurnReply : BasicReply<PlayerAction::END_TURN> {};

struct ExitGameReply : BasicReply<PlayerAction::EXIT_GAME> {};

struct TakeCardReply : BasicReply<PlayerAction::TAKE_CARD> {};

struct UseCardReply : BasicReply<PlayerAction::USE_CARD> {};

#endif //PLAYERREQUESTS_H
