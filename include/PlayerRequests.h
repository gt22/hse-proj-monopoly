#ifndef PLAYERREQUESTS_H
#define PLAYERREQUESTS_H

#include <vector>
#include <string>
#include <memory>

enum class PlayerAction {
    PAY_TAX,
    BUY_FIELD,
    BUY_HOUSE,
    BUY_HOTEL,
    USE_CARD,
    PAY_TO_OTHER_PLAYER,
    TAKE_CARD,
    MORTGAGE_HOLDINGS,
    BUY_BACK_PROPERTY,
    START_TRADE,
    START_TRADE_NEW_FIELD,
    ROLL_DICE,
    END_TURN,
    EXIT_GAME,
    SELL_HOUSE,
    SELL_FIELD,
    SELL_HOTEL,
    LOSE
};

enum class PlayerTradeAction {
    REFUSE,
    PARTICIPATE
};

struct PlayerRequest final {
    PlayerRequest();
    explicit PlayerRequest(std::vector<PlayerAction> availableActions,
            std::string_view message);
    std::vector<PlayerAction> availableActions;
    std::string_view message;
};

struct PlayerTradeRequest final {
    PlayerTradeRequest();
    explicit PlayerTradeRequest(std::string_view message);
    std::string_view message;
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

struct NumReplyData {
    explicit NumReplyData(std::size_t num);
    virtual ~NumReplyData() = default;
    std::size_t num;
};

struct SumReplyData {
    explicit SumReplyData(int amount);
    virtual ~SumReplyData() = default;
    int amount;
};

struct PlayerTradeReplyData {
    explicit PlayerTradeReplyData(PlayerTradeAction action);
    virtual ~PlayerTradeReplyData() = default;
    const PlayerTradeAction action;
};

template<PlayerAction ACTION>
struct BasicReply : PlayerReplyData {
    BasicReply() : PlayerReplyData(ACTION) {}
};

struct RollDiceReply : BasicReply<PlayerAction::ROLL_DICE> {};

struct PayTaxReply : BasicReply<PlayerAction::PAY_TAX> {};

struct PayToOtherPlayersReply : BasicReply<PlayerAction::PAY_TO_OTHER_PLAYER> {};

struct BuyPropertyReply : BasicReply<PlayerAction::BUY_FIELD> {};

struct SellPropertyReply : BasicReply<PlayerAction::SELL_FIELD> {};

struct BuyBuildingReply : BasicReply<PlayerAction::BUY_HOUSE> {};

struct SellBuildingReply : BasicReply<PlayerAction::SELL_HOUSE> {};

struct BuyHotelReply : BasicReply<PlayerAction::BUY_HOTEL> {};

struct SellHotelReply : BasicReply<PlayerAction::SELL_HOTEL> {};

struct EndTurnReply : BasicReply<PlayerAction::END_TURN> {};

struct ExitGameReply : BasicReply<PlayerAction::EXIT_GAME> {};

struct TakeCardReply : BasicReply<PlayerAction::TAKE_CARD> {};

struct UseCardReply : BasicReply<PlayerAction::USE_CARD> {};

struct StartTradeReply : BasicReply<PlayerAction::START_TRADE> {};

struct MortgageHoldingsReply : BasicReply<PlayerAction::MORTGAGE_HOLDINGS> {};



#endif //PLAYERREQUESTS_H
