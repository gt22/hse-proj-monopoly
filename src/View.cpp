#include <ncurses.h>
#include <string>
#include "View.h"

AbstractView::AbstractView(Manager& manager) : manager(manager) {}

MenuView::MenuView(Manager &manager) : AbstractView(manager) {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
}

MenuView::~MenuView() {
    endwin();
}

void addPlayerMenu() {

}

void MenuView::menuInteraction() {
    getmaxyx(stdscr, menuSizeY, menuSizeX);
    char buttonsList[NUMBER_OF_BUTTONS][15] = { "Add player", "Start game", "Exit" };
    char button[15];
    int i;
    menuSizeX--, menuSizeY--;

    WINDOW* menuWindow = newwin(menuSizeY, menuSizeX, 1, 1);
   // box(menuWindow, 0, 0);
    int y = 2 * menuSizeY / 6;
    int x = 2 * menuSizeX / 6;
    int yi = y;
    for(i = 0; i < NUMBER_OF_BUTTONS; i++) {
        if (i == 0)
            wattron(menuWindow, A_STANDOUT);
        else
            wattroff(menuWindow, A_STANDOUT);
        sprintf(button, "%s",  buttonsList[i]);
        mvwprintw(menuWindow, yi + 1, x, "%s", button);
        yi++;
    }

    wrefresh(menuWindow);

    i = 0, yi = y;
    keypad(menuWindow, TRUE);
    curs_set(0);
    bool flag = false;
    while(true) {
        int ch = wgetch(menuWindow);
        sprintf(button, "%s",  buttonsList[i]);
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
                   // WINDOW* playerWindow = newwin(menuSizeY / 2, menuSizeX / 2, menuSizeY / 2, menuSizeX / 2);
                   // keypad(playerWindow, TRUE);
                   // manager.addPlayer();
                  // addPlayerMenu();
                } else if (i == 1) {
                    // view.runGame();
                } else {
                    flag = true;
                }
                break;
        }
        wattron(menuWindow, A_STANDOUT);

        sprintf(button, "%s",  buttonsList[i]);
        mvwprintw(menuWindow, yi + 1, x, "%s", button);
        wattroff(menuWindow, A_STANDOUT);

        if (flag)
            break;
    }

    delwin(menuWindow);
}

MonopolyView::MonopolyView(Manager &manager) : AbstractView(manager) {}

NcursesView::NcursesView(Manager& manager) : MonopolyView(manager) {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
}


NcursesView::~NcursesView() {
    endwin();
}

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
    move(fieldSizeY, fieldSizeX);
    refresh();
}


bool NcursesView::getActionAlpha() {
    int enteredChar = getch();
    if (enteredChar == 'x')
        return true;
    return false;
}

void NcursesView::runGame() {
    while (true) {
        bool flag = false;
		while (true) {
            printGrid();
			if (getActionAlpha()) { 
				flag = true;
                break;
            }
		}
        if (flag)
            break;
    }
}

PlayerReply NcursesView::processRequest(Player& player, PlayerRequest req) {
    static_cast<void>(player);
    return {req.message.empty()};
}

void NcursesView::redraw(const Board& board) {
    static_cast<void>(board);
}


