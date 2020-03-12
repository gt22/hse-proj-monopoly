#include "Player.h"

#include <utility>
#include "Cards.h"
#include "View.h"
Player::Player(Token token) : token(token) {}

LocalPlayer::LocalPlayer(Token token, std::shared_ptr<ViewHolder> view)
: Player(token), view(std::move(view)) {

}

PlayerReply LocalPlayer::sendRequest(PlayerRequest request) {
    return gameView().processRequest(*this, request);
}

void LocalPlayer::sync(const Board &board) {
    gameView().redraw(board);
}

MonopolyView &LocalPlayer::gameView() {
    return dynamic_cast<MonopolyView&>(**view);
}
