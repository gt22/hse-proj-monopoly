#include "Game.h"
#include "time.h"

Game::Game(const std::vector<std::pair<std::string, Token>>& players, std::shared_ptr<Manager> manager) :
            board(players, *this), manager(manager){

}

PlayerReply Game::sendRequest(Token token, PlayerRequest request) {
    return manager->Manager::sendRequest(token, request);
}

void Game::runGame() {
    srand(time(0));
    std::size_t curPlayerNum = 0;
    while (true) {
        PlayerData curPlayer = board.getPlayer(board.getPlayerToken(curPlayerNum));
        if (curPlayer.prisoner) {
            board.field[curPlayer.position]->onPlayerEntry(curPlayer.token);
            //double => make turn(without throwing dice)
            //special card
            continue;
        }
        int firstTrow = rand() % 6 + 1, secondTrow = rand() % 6 + 1;
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