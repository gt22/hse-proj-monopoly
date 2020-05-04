#ifndef PLAYERREQUESTS_H
#define PLAYERREQUESTS_H

#include <vector>
#include <string>
#include <memory>

enum class PlayerAction {
    PAY_TAX,
    BUY_PROPERTY,
    BUY_BUILDING,
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
    LOSE
};

struct PlayerRequest final {
    PlayerRequest();
    explicit PlayerRequest(std::vector<PlayerAction> availableActions,
            std::string_view message);
    std::vector<PlayerAction> availableActions;
    std::string_view message;
};

struct PlayerMessage final {
    explicit PlayerMessage(std::string_view message);
    std::string_view message;
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

struct PayTaxReply : BasicReply<PlayerAction::PAY_TAX> {};

struct BuyPropertyReply : BasicReply<PlayerAction::BUY_PROPERTY> {};

struct BuyBuildingReply : BasicReply<PlayerAction::BUY_BUILDING> {};

struct EndTurnReply : BasicReply<PlayerAction::END_TURN> {};

struct ExitGameReply : BasicReply<PlayerAction::EXIT_GAME> {};

typedef std::unique_ptr<PlayerReplyData> PlayerReply;

#endif //PLAYERREQUESTS_H
