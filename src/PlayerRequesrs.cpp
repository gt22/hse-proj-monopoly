#include "PlayerRequests.h"

PlayerRequest::PlayerRequest() : PlayerRequest({}, "") {}

PlayerRequest::PlayerRequest(std::vector<PlayerAction> availableActions,
        std::string message)
 : availableActions(std::move(availableActions)), message(message) {}

PlayerMessage::PlayerMessage(std::string message) : message(message) {}

PlayerReplyData::PlayerReplyData(PlayerAction action) : action(action) {}