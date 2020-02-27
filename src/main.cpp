#include "MonopolyFwd.h"
#include "Board.h"

#include <iostream>
#include <vector>
#include <map>

int throwDice() {
    return 6;
}

int main() {
    bool notFinished = true;
    int curPlayer = 0;
    std::vector<std::pair<std::string, Token> > playersList;
    //TODO: считать игроков
    std::map<int, Token> getToken;//вынести в функцию(?)
    Board board(playersList);
    while (notFinished) {
        curPlayer = (curPlayer + 1) % board.getNumberOfPlayers();
        PlayerData& player = board.getPlayer(getToken[curPlayer]);
        if (player.skip) {
            //TODO: в тюрьме, надо бросить кубики(если не повезло, остается в тюрьме?)
            //там вроде еще карточки такие бывают. на 1 круг
            player.skip = false;
            continue;
        }
        int firstThrow = throwDice();
        int secondThrow = throwDice();
        bool toPrison = player.checkDouble(firstThrow == secondThrow);
        if (toPrison) {
            //TODO
            continue;
        }

    }
    return 0;
}
