#include "Player.h"

#include <utility>
#include "Cards.h"
#include "view/SFMLView.h"
Player::Player(Token token, std::string name) : token(token), name(std::move(name)) {}

LocalPlayer::LocalPlayer(Token token, std::string name, SFMLView& view)
: Player(token, std::move(name)), view(view) {

}

PlayerReply LocalPlayer::sendRequest(PlayerRequest request) {
    return gameView().processRequest(*this, request);
}

void LocalPlayer::sendMessage(PlayerMessage request, MessageType type) {
    gameView().processMessage(*this, std::move(request), type);
}

NumReply LocalPlayer::sendNum() {
    return gameView().processNum(*this);
}

void LocalPlayer::sync(const Board &board) {
    gameView().redraw(board);
}

SFMLView& LocalPlayer::gameView() {
    return view;
}

PlayerTradeReply LocalPlayer::sendTradeRequest(PlayerTradeRequest request) {
    return gameView().processTradeRequest(*this, request);
}

SumReply LocalPlayer::sendSum() {
    return gameView().processSum(*this);
}


