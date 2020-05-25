#include "Player.h"

#include <utility>
#include "Cards.h"
#include "view/SFMLView.h"
Player::Player(Token token, std::string name) : token(token), name(std::move(name)) {}


LocalPlayer::LocalPlayer(Token token, std::string name, SFMLView& view)
: Player(token, std::move(name)), view(view) {

}

PlayerReply LocalPlayer::send(PlayerRequest request) {
    return gameView().processRequest(*this, std::move(request));
}

void LocalPlayer::sync(const Board &board) {
    gameView().redraw(board);
}

SFMLView& LocalPlayer::gameView() {
    return view;
}