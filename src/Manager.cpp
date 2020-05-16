#include "Manager.h"
#include "Game.h"
#include "view/SFMLView.h"
#include <string_view>
#include <cassert>

void Manager::addPlayer(std::unique_ptr<Player> player) {
    players.push_back(std::move(player));
}

PlayerReply Manager::sendRequest(Token token, PlayerRequest request) {
    for(auto& p : players) {
        if(p->token == token) {
            return p->sendRequest(std::move(request));
        }
    }
    throw 1; //TODO: сделать PlayerNotFoundException или что-нибудь такое
}

NumReply Manager::sendNumRequest(Token token) {
    for(auto& p : players) {
        if(p->token == token) {
            return p->sendNum();
        }
    }
    throw 1; //TODO: сделать PlayerNotFoundException или что-нибудь такое
}

void Manager::sendMessage(Token token, PlayerMessage mes, MessageType type) {
    for(auto& p : players) {
        if(p->token == token) {
            p->sendMessage(std::move(mes), type);
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
    game = std::make_shared<Game>(gameData, *this);
}

//TODO should be const, non-const only for debug
Board& Manager::getBoard() {
    return game->getBoard();
}

void Manager::sync(const Board& board) {
    for(auto& p : players) {
        p->sync(board);
    }
}

void Manager::startGame() {
    gameThread = std::thread([this](){
        createGame();
        sync(game->getBoard());
        game->runGame();
    });
}

void Manager::createView() {
    view = std::make_shared<SFMLView>(*this);
}

void Manager::run() {
    createView();
    view->mainLoop();
}

bool Manager::isGameStarted() {
    return bool(game);
}

Manager::~Manager() {
    view->dispose();
    if(gameThread.joinable()) {
        gameThread.join();
    }
}




