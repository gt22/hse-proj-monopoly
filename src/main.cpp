#include "MonopolyFwd.h"
#include "Manager.h"
#include "Board.h"

#include <iostream>
#include "View.h"

int main() {
    Manager manager;
    MenuView menu(manager);
    menu.menuInteraction();
    NcursesView view(manager);
    view.runGame();
    return 0;
}

