#include "MonopolyFwd.h"
#include "Manager.h"
#include "Board.h"
#include "include/network/internal/InternalSerialization.h"
#include <iostream>
#include "View.h"

int main() {
    setlocale(LC_ALL, "");
    Manager manager;
    manager.createGame();
    MenuView menu(manager);
    menu.menuInteraction();
    NcursesView view(manager);
    view.runGame();
    return 0;
}

