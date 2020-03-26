#include "PlayerRequests.h"

PlayerRequest::PlayerRequest() : PlayerRequest({}, "") {}

PlayerRequest::PlayerRequest(std::vector<PlayerAction> availableActions,
        std::string_view message)
 : availableActions(std::move(availableActions)), message(message) {}

PlayerMessage::PlayerMessage(std::string_view message) : message(message) {}

PlayerReplyData::PlayerReplyData(PlayerAction action) : action(action) {}