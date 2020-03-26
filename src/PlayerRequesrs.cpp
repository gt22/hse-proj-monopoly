#include "PlayerRequests.h"

PlayerRequest::PlayerRequest(std::vector<PlayerAction> availableActions,
        std::string message)
 : availableActions(std::move(availableActions)), message(std::move(message)) {}

PlayerMessage::PlayerMessage(std::string message) : message(std::move(message)) {}

PlayerReplyData::PlayerReplyData(PlayerAction action) : action(action) {}