#include "Game.h"

#include <utility>
#include "Manager.h"

Game::Game(const std::vector<std::pair<std::string_view, Token>> &players, Manager& manager)
    : board(players, *this), manager(manager) {

}

PlayerReply Game::sendRequest(Token token, PlayerRequest request) {
    return manager.sendRequest(token, std::move(request));
}
