#include "Menu.h"
#include "Manager.h"

MenuView::MenuView(Manager &manager) : AbstractView(manager) {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

MenuView::~MenuView() {
    endwin();
}

void MenuView::addPlayerMenu() {
    /* Read name */
    int y = 2 * menuSizeY / 6 - 2;
    int x = 4 * menuSizeX / 6;

    /*   nocbreak();
       echo();

       std::string name;
       mvwprintw(menuWindow, y, x, "NAME: ");
       wrefresh(menuWindow);
       int ch = wgetch(menuWindow);

       while (ch != '\n') {
           name.push_back(ch);
           ch = wgetch(menuWindow);
       }
       noecho();
       cbreak();
       wrefresh(menuWindow); */

    /* Choose token */
    int tokenY = y, tokenX = x;
    mvwprintw(menuWindow, y, x, "CHOOSE YOUR FIGHTER:");
    tokenY++;
    std::string buttonsList[NUMBER_OF_TOKENS]= { "DOG", "HAT", "BOOT", "CAT", "CAR", "SHIP" };
    char button[5];
    int i, yi = tokenY;
    for(i = 0; i < NUMBER_OF_TOKENS; i++) {
        if (i == 0)
            wattron(menuWindow, A_STANDOUT);
        else
            wattroff(menuWindow, A_STANDOUT);
        sprintf(button, "%s",  buttonsList[i].c_str());
        mvwprintw(menuWindow, yi + 1, tokenX, "%s", button);
        yi++;
    }

    wrefresh(menuWindow);

    i = 0, yi = tokenY;
    std::string token;
    bool flag = false;

    while(true) {
        int ch = wgetch(menuWindow);
        sprintf(button, "%s", buttonsList[i].c_str());
        mvwprintw(menuWindow, yi + 1, tokenX, "%s", button);
        switch (ch) {
            case KEY_UP:
                i--;
                if (i < 0) {
                    i = NUMBER_OF_TOKENS - 1;
                    yi = tokenY + NUMBER_OF_TOKENS - 1;
                } else {
                    yi--;
                }
                break;
            case KEY_DOWN:
                i++;
                if (i > NUMBER_OF_TOKENS - 1) {
                    i = 0;
                    yi = tokenY;
                } else {
                    yi++;
                }
                break;
            case '\n':
                if (i == 0) {
                    manager.addPlayer(std::make_unique<LocalPlayer>(Token::DOG, manager.view));
                } else if (i == 1) {
                    manager.addPlayer(std::make_unique<LocalPlayer>(Token::HAT, manager.view));
                } else if (i == 2) {
                    manager.addPlayer(std::make_unique<LocalPlayer>(Token::BOOT, manager.view));
                } else if (i == 3) {
                    manager.addPlayer(std::make_unique<LocalPlayer>(Token::CAT, manager.view));
                } else if (i == 4) {
                    manager.addPlayer(std::make_unique<LocalPlayer>(Token::CAR, manager.view));
                } else if (i == 5) {
                    manager.addPlayer(std::make_unique<LocalPlayer>(Token::SHIP, manager.view));
                }
                flag = true;
                break;
        }
        wattron(menuWindow, A_STANDOUT);

        sprintf(button, "%s",  buttonsList[i].c_str());
        mvwprintw(menuWindow, yi + 1, x, "%s", button);
        wattroff(menuWindow, A_STANDOUT);
        if (flag)
            break;
    }

    for (yi = y; yi < menuSizeY; yi++)
        for (int xi = x; xi < menuSizeX; xi++)
            mvwprintw(menuWindow, yi, xi, " ");
}

MenuAction MenuView::menuInteraction() {
    getmaxyx(stdscr, menuSizeY, menuSizeX);
    std::string buttonsList[NUMBER_OF_BUTTONS] = { "Add player", "Start game", "Exit" };
    char button[15];
    int i;
    menuSizeX--, menuSizeY--;

    menuWindow = newwin(menuSizeY, menuSizeX, 1, 1);
    // box(menuWindow, 0, 0);
    int y = 2 * menuSizeY / 6;
    int x = 2 * menuSizeX / 6;
    int yi = y;
    for(i = 0; i < NUMBER_OF_BUTTONS; i++) {
        if (i == 0)
            wattron(menuWindow, A_STANDOUT);
        else
            wattroff(menuWindow, A_STANDOUT);
        sprintf(button, "%s",  buttonsList[i].c_str());
        mvwprintw(menuWindow, yi + 1, x, "%s", button);
        yi++;
    }

    wrefresh(menuWindow);

    i = 0, yi = y;
    keypad(menuWindow, TRUE);
    bool flag = false;
    while(true) {
        int ch = wgetch(menuWindow);
        sprintf(button, "%s",  buttonsList[i].c_str());
        mvwprintw(menuWindow, yi + 1, x, "%s", button);
        switch (ch) {
            case KEY_UP:
                i--;
                if (i < 0) {
                    i = NUMBER_OF_BUTTONS - 1;
                    yi = y + NUMBER_OF_BUTTONS - 1;
                } else {
                    yi--;
                }
                break;
            case KEY_DOWN:
                i++;
                if (i > NUMBER_OF_BUTTONS - 1) {
                    i = 0;
                    yi = y;
                } else {
                    yi++;
                }
                break;
            case '\n':
                if (i == 0) {
                    // manager.addPlayer();
                    addPlayerMenu();

                } else if (i == 1) {
                    clear();
                    return MenuAction::START_GAME;
                } else {
                    flag = true;
                }
                break;
        }
        wrefresh(menuWindow);
        wattron(menuWindow, A_STANDOUT);

        sprintf(button, "%s",  buttonsList[i].c_str());
        mvwprintw(menuWindow, yi + 1, x, "%s", button);
        wattroff(menuWindow, A_STANDOUT);

        if (flag)
            return MenuAction::EXIT;
    }

    delwin(menuWindow);
}
