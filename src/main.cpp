#include <iostream>
#include "view.h"

int main() {
    
    MenuView menu;
    menu.MenuInteraction();
    NcursesView view;
    view.runGame();
        //std::cout << "Hello, World!" << std::endl;
    return 0;
}

// Из игроков нужна position, token
// Массив указателей Board.Field
