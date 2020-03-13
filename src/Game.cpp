#include "Game.h"
#include "Field.h"
#include <utility>
#include "Manager.h"

Game::Game(const std::vector<std::pair<std::string_view, Token>> &players, Manager& manager)
    : board(players, *this), manager(manager) {

}

PlayerReply Game::sendRequest(Token token, PlayerRequest request) {
    return manager.sendRequest(token, std::move(request));
}

void Game::runGame() {
    std::size_t curPlayerNum = 0;
    while (true) {
        PlayerData& curPlayer = board.getPlayer(board.getPlayerToken(curPlayerNum));
        if (!curPlayer.alive) {
            curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
            continue;
        }
        if (curPlayer.prisoner) {
            board.field[curPlayer.position]->onPlayerEntry(curPlayer.token);
            //double => make turn(without throwing dice)
            //special card
            continue;
        }
        int firstTrow = rng.nextInt(1, 6), secondTrow = rng.nextInt(1, 6);
        bool extraTurn = false;
        if (firstTrow == secondTrow) {
            curPlayer.doubleDice++;
            extraTurn = true;
        } else {
            curPlayer.doubleDice = 0;
        }
        if (curPlayer.doubleDice == 3) {
            curPlayer.toPrison();
        } else {
            curPlayer.newPosition(firstTrow + secondTrow);
        }
        board.field[curPlayer.position]->onPlayerEntry(curPlayer.token);
        if (!extraTurn) {
            curPlayerNum = (curPlayerNum + 1) % board.getPlayersNumber();
        }
        break;
    }
}