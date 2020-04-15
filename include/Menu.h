#ifndef MENU_H
#define MENU_H

#include "View.h"

enum class MenuAction {
    START_GAME,
    EXIT
};

class MenuView final : public AbstractView {
public:
    explicit MenuView(Manager& manager);
    ~MenuView() override;
    MenuAction menuInteraction();
    int getPlayersNumber();
    void addPlayerMenu();
private:
    WINDOW* menuWindow;
    int menuSizeX = 0;
    int menuSizeY = 0;
    static const int NUMBER_OF_BUTTONS = 3;
    static const int NUMBER_OF_TOKENS = 6;
};

#endif // MENU_H
