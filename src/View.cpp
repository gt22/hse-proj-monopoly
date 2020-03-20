#include <ncurses.h>
#include <string>
#include "Player.h"
#include "View.h"
#include "Manager.h"
#include <memory>

AbstractView::AbstractView(Manager& manager) : manager(manager) {}

// MENU VIEW

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

void MenuView::menuInteraction() {
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
                    // view.runGame();
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
            break;
    }

    delwin(menuWindow);
}


// GAME VIEW

MonopolyView::MonopolyView(Manager &manager) : AbstractView(manager) {}

NcursesView::NcursesView(Manager& manager) : MonopolyView(manager) {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

NcursesView::~NcursesView() {
    endwin();
}

// PLAYER
void NcursesView::getCoord(int &x, int &y, int index) {
    if (index >= 40)
        index %= 40;
    if (index < TILES_PER_LINE) {
        x = tileSizeX / 2 + (index) * tileSizeX;
        y = tileSizeY / 2;
    } else if (index < 2 * TILES_PER_LINE - 1) {
        x = fieldSizeX - tileSizeX / 2;
        y = tileSizeY / 2 + (index - TILES_PER_LINE + 1) * tileSizeY;
    } else if (index < 3 * TILES_PER_LINE - 2) {
        x = fieldSizeX - tileSizeX / 2 - (index - 2 * TILES_PER_LINE + 2) * tileSizeX;
        y = fieldSizeY - tileSizeY / 2;
    } else {
        x = tileSizeX / 2;
        y = fieldSizeY - tileSizeY / 2 - (index - 3 * TILES_PER_LINE + 3) * tileSizeY;
    }
}

void NcursesView::drawPlayer(int x, int y, int color) {
    if(has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, color, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    mvaddch(y, x, ' '|A_REVERSE);
    attroff(COLOR_PAIR(1));
}

// FIELD

void NcursesView::changeFieldSize() {
    while (maxX % TILES_PER_LINE != 0 && maxX > 0) {
        maxX--;
    }
    while (maxY % TILES_PER_LINE != 0 && maxY > 0) {
        maxY--;
    }
    fieldSizeX = maxX;
    fieldSizeY = maxY;
    tileSizeY = maxY / TILES_PER_LINE;
    tileSizeX = maxX / TILES_PER_LINE;
}

void NcursesView::printGrid() {
    getmaxyx(stdscr, maxY, maxX);
    maxX--;
    maxY--;
    changeFieldSize();

    /* right and left lines*/
    for (int line = 0; line <= fieldSizeX - tileSizeX; line +=(fieldSizeX - tileSizeX)) {
        for (int x = 0; x <= tileSizeX; x += ( tileSizeX))
            for (int y = 0; y <= fieldSizeY; y++) {
                move(y, line + x);
                addch(ACS_VLINE);
            }
    }

    /*bottom and top lines*/
    for (int line = 0; line <= fieldSizeY - tileSizeY; line += (fieldSizeY - tileSizeY)) {
        for (int y = 0; y <= tileSizeY; y+=tileSizeY)
            for (int x = 1; x < fieldSizeX; x++) {
                move(line + y, x);
                addch(ACS_HLINE);
            }
    }
    
    /*cells of right and left */ 
    for (int line = 0; line <= fieldSizeX - tileSizeX; line +=(fieldSizeX - tileSizeX)) {
        for (int y = 0; y <= (fieldSizeY); y+=tileSizeY)
            for (int x = 1; x < tileSizeX; x++) {
                move(y, line + x);
                addch(ACS_HLINE);
            }
    }

     /*cells of bottom and top lines*/
    for (int line = 0; line <= fieldSizeY - tileSizeY; line += (fieldSizeY - tileSizeY)) {
        for (int x = 0; x <= fieldSizeX; x+=tileSizeX)
                for (int y = 1; y < tileSizeY; y++) {
                    move(y + line, x);
                    addch(ACS_VLINE);
                }
    }

    /* corners */
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, fieldSizeX, ACS_URCORNER);
    mvaddch(fieldSizeY, 0, ACS_LLCORNER);
    mvaddch(fieldSizeY, fieldSizeX, ACS_LRCORNER);

    for (int x = 0; x <= fieldSizeX - tileSizeX; x+=(fieldSizeX - tileSizeX))
        for (int y = tileSizeY; y < fieldSizeY; y+=tileSizeY)
            mvaddch(y, x, ACS_LTEE);

    for (int x = tileSizeX; x <= fieldSizeX; x+=(fieldSizeX - tileSizeX))
        for (int y = tileSizeY; y < fieldSizeY; y+= tileSizeY)
            mvaddch(y, x, ACS_RTEE);

    for (int y = 0; y<= fieldSizeY - tileSizeY; y+=(fieldSizeY - tileSizeY))
        for (int x = tileSizeX; x < fieldSizeX; x+=tileSizeX)
            mvaddch(y, x, ACS_TTEE);

    for (int y = tileSizeY; y<= fieldSizeY; y+=(fieldSizeY - tileSizeY))
        for (int x = tileSizeX; x < fieldSizeX; x+=tileSizeX)
            mvaddch(y, x, ACS_BTEE);

    /* crosses */
    for (int y = tileSizeY; y <= fieldSizeY - tileSizeY; y += fieldSizeY - 2 * tileSizeY)
        for (int x = tileSizeX; x <= fieldSizeX - tileSizeX; x += fieldSizeX - 2 * tileSizeX) {
            move(y, x);
            addch(ACS_PLUS);
        }
    refresh();
}


void NcursesView::runGame() {
    int ind = 0;
    while (true) {
        bool flag = false;
        printGrid();
        int x = 0, y = 0;
        int enteredChar = getch();

        switch(enteredChar) {
            case 'x':
                flag = true;
                break;
            case 'd':
                erase();
                getCoord(x, y, ind);
                drawPlayer(x, y, COLOR_CYAN);
                break;
            case KEY_RIGHT:
                erase();
                refresh();
                ind++;
                refresh();
                getCoord(x, y, ind);
                drawPlayer(x, y, COLOR_CYAN);
                refresh();
                break;
        }
        refresh();
        if (flag)
            break;
    }
}

PlayerReply NcursesView::processRequest(Player& player, PlayerRequest req) {
    return std::make_unique<EndTurnReply>();
}

void NcursesView::redraw(const Board& board) {

}

