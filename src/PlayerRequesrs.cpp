#include "PlayerRequests.h"

PlayerRequest::PlayerRequest(std::vector<PlayerAction> availableActions,
        std::string message)
 : availableActions(std::move(availableActions)), message(std::move(message)) {}

PlayerReplyData::PlayerReplyData(PlayerAction action) : action(action) {}

EndTurnReply::EndTurnReply() : PlayerReplyData(PlayerAction::END_TURN) {}

PayTaxReply::PayTaxReply() : PlayerReplyData(PlayerAction::PAY_TAX) {}
