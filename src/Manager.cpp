#include "Manager.h"
#include "Game.h"
#include <string_view>
#include <cassert>

#include <iostream>

void Manager::addPlayer(std::unique_ptr<Player> player) {
    players.push_back(std::move(player));
}

PlayerReply Manager::sendRequest(Token token, PlayerRequest request) {
    std::cerr << "Request-token: " << (int) token << std::endl;
    for(auto& p : players) {
        if(p->token == token) {
            return p->sendRequest(std::move(request));
        }
    }
    throw 1; //TODO: сделать PlayerNotFoundException или что-нибудь такое
}

void Manager::sendMessage(Token token, PlayerMessage mes) {
    std::cerr << "Message-token: " << (int) token << ", size: " << players.size() << std::endl;
    for(auto& p : players) {
        if(p->token == token) {
            p->sendMessage(std::move(mes));
            return;
        }
    }
    throw 1; //TODO: сделать PlayerNotFoundException или что-нибудь такое
}

void Manager::createGame() {
    assert(!game);
    std::vector<std::pair<std::string_view, Token>> gameData;
    gameData.reserve(players.size());
    for(const auto& p : players) {
        gameData.emplace_back(std::string_view (p->name), p->token);
    }
    game = std::make_unique<Game>(gameData, *this);
}

const Board &Manager::getBoard() {
    return game->getBoard();
}

void Manager::sync(const Board& board) {
    for(auto& p : players) {
        p->sync(board);
    }
}

void Manager::createMenu() {
    view = std::make_shared<ViewHolder>(nullptr);
    *view = std::make_unique<TerminalMenu>(*this);
    auto& menu = static_cast<TerminalMenu&>(**view);
    switch(menu.menuInteraction()) {
        case MenuAction::START_GAME: {
            (*view).reset();
            startGame();
            break;
        }
        case MenuAction::EXIT:
            return;
    }

}

void Manager::startGame() {
    createGame();
    *view = std::make_unique<NcursesView>(*this);
    sync(game->getBoard());
    game->runGame();
}



