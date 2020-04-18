#include "MonopolyThreadManager.h"
#include "Player.h"

void MonopolyThreadManager::run() {
    runUIOnMain();
}

void MonopolyThreadManager::startGame() {

}


void MonopolyThreadManager::runLogic() {

}

void MonopolyThreadManager::runUIOnMain() {
//    view = std::make_shared<SFMLView>(*this);
    view->mainLoop();
}

void MonopolyThreadManager::addPlayer(std::unique_ptr<Player> player) {
    Token tok = player->token;
    players.emplace(tok, std::move(player));
}
