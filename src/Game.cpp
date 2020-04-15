#include "Game.h"
#include "Field.h"
#include <utility>
#include "Manager.h"

Game::Game(const std::vector<std::pair<std::string_view, Token>> &players, Manager& manager)
    : board(players, *this), manager(manager) {

}

PlayerReply Game::sendRequest(Token token, PlayerRequest request) {
    sync();
    return manager.sendRequest(token, std::move(request));
}

void Game::sendMessage(Token token, PlayerMessage mes) {
    sync();
    manager.sendMessage(token, std::move(mes));
}

void Game::runGame() {
    std::size_t curPlayerNum = 0;
    int numOfAlivePlayers = board.getPlayersNumber();
    while (!board.isFinished) {
        PlayerData& curPlayer = board.getPlayer(board.getPlayerToken(curPlayerNum));
        if (!curPlayer.alive) {
            curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
            continue;
        }
        if (numOfAlivePlayers == 1) {
            sendMessage(curPlayer.token, PlayerMessage("Victory!"));
            sync();
            board.isFinished = true;
        }
        if (curPlayer.prisoner) {
            board.field[curPlayer.position]->onPlayerEntry(curPlayer.token);
            curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
            continue;
        }
        int firstTrow = rng.nextInt(1, 6), secondTrow = rng.nextInt(1, 6);
        curPlayer.lastTrow = firstTrow + secondTrow;
        sendMessage(curPlayer.token,
                PlayerMessage(std::to_string(firstTrow) + " " + std::to_string(secondTrow)));
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
}

const Board &Game::getBoard() {
    return board;
}

void Game::sync() {
    manager.sync(board);
}
