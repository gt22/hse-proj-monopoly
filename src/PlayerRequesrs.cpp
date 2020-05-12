#include "PlayerRequests.h"

PlayerRequest::PlayerRequest() : PlayerRequest({}, "") {}

PlayerRequest::PlayerRequest(std::vector<PlayerAction> availableActions,
        std::string_view message)
 : availableActions(std::move(availableActions)), message(message) {}

PlayerMessage::PlayerMessage(std::string message) : message(std::move(message)) {}

PlayerReplyData::PlayerReplyData(PlayerAction action) : action(action) {}

NumReplyData::NumReplyData(std::size_t num) : num(num) {}
