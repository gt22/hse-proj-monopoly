#include "Game.h"
#include "Field.h"
#include <utility>
#include "Manager.h"


Game::Game(const std::vector<std::pair<std::string_view, Token>> &players, Manager& manager)
    : board(players, *this), manager(manager) { }

PlayerReply Game::sendRequest(Token token, PlayerRequest request) {
    sync();
    PlayerReply rep = manager.sendRequest(token, std::move(request));
    if(rep->action == PlayerAction::EXIT_GAME) getBoard().terminate();
    return rep;
}

void Game::sendMessage(Token token, PlayerMessage mes, MessageType type) {
    sync();
    manager.sendMessage(token, std::move(mes), type);
}

void Game::runGame() {
    std::size_t curPlayerNum = 0;
    while (!board.isFinished()) {
        board.setPlayerIndex(curPlayerNum);
        PlayerData& curPlayer = board.getPlayer(board.getPlayerToken(curPlayerNum));

        if (!curPlayer.alive) {
            curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
            continue;
        }
        PlayerRequest startTurnRequest;
        startTurnRequest.availableActions.push_back(PlayerAction::ROLL_DICE);
        PlayerReply reply = board.sendRequest(curPlayer.token, startTurnRequest);
        if (curPlayer.prisoner) {
            board.field[curPlayer.position]->onPlayerEntry(curPlayer.token);
            curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
            continue;
        }
        int firstTrow = rng.nextInt(1, 6), secondTrow = rng.nextInt(1, 6);
        curPlayer.lastTrow = firstTrow + secondTrow;
        sendMessage(curPlayer.token,
                PlayerMessage(std::to_string(firstTrow) + " " + std::to_string(secondTrow)), MessageType::DICE);
        bool extraTurn = false;
        if (firstTrow == secondTrow) {
            curPlayer.doubleDice++;
            extraTurn = true;
        } else {
            curPlayer.doubleDice = 0;
        }
        if (curPlayer.doubleDice == 3) {
            extraTurn = false;
            curPlayer.toPrison();
        } else {
            curPlayer.newPosition(firstTrow + secondTrow);
        }
        board.field[curPlayer.position]->onPlayerEntry(curPlayer.token);
        if (!extraTurn) {
            curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
        }
        sync();
    }
    board.sendMessage(board.getWinner(), PlayerMessage("Victory!"), MessageType::INFO);
    sync();
}

//TODO should be const, non-const only for debug
Board& Game::getBoard() {
    return board;
}

void Game::sync() {
    manager.sync(board);
}

NumReply Game::sendNumRequest(Token token) {
    sync();
    NumReply rep = manager.sendNumRequest(token);
    return rep;
}
