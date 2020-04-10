#include "MonopolyFwd.h"
#include "Manager.h"
#include "Board.h"
#include "locale.h"
#include <iostream>
#include "View.h"

int main() {
    setlocale(LC_ALL, "");
    Manager manager;
    MenuView menu(manager);
    menu.menuInteraction();
 //   NcursesView view(manager);
   // view.runGame();
    return 0;
}

