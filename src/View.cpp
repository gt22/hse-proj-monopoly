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

void MenuView::menuInteraction() {
    getmaxyx(stdscr, menuSizeY, menuSizeX);
    char list[NUMBER_OF_BUTTONS][15] = { "Add player", "Start game", "Exit" };

    char item[15];
    int ch, i;
    menuSizeX--, menuSizeY--;
    WINDOW* w = newwin( menuSizeY, menuSizeX, 1, 1 );
    box(w, 0, 0);
    int y = menuSizeY / 2;
    int x = menuSizeX / 2;
    int yi = y;
    for(i = 0; i < NUMBER_OF_BUTTONS; i++) {
        if (i == 0)
            wattron(w, A_STANDOUT);
        else
            wattroff(w, A_STANDOUT);
        sprintf(item, "%s",  list[i]);
        mvwprintw(w, yi + 1, x, "%s", item);
        yi++;
    }

    wrefresh(w);

    i = 0, yi = y;
    keypad(w, TRUE);
    curs_set(0);
    bool flag = false;
    while(true) {
        ch = wgetch(w);
        sprintf(item, "%s",  list[i]);
        mvwprintw(w, yi + 1, x, "%s", item);
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
                    // addPlayer();
                } else if (i == 1) {
                    // view.runGame();
                } else {
                    flag = true;
                }
                break;
        }
        wattron( w, A_STANDOUT );

        sprintf(item, "%s",  list[i]);
        mvwprintw( w, yi + 1, x, "%s", item);
        wattroff(w, A_STANDOUT);

        if (flag)
            break;
    }

    delwin(w);
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
                printw("|");
            }
    }

    /*bottom and top lines*/
    for (int line = 0; line <= fieldSizeY - tileSizeY; line += (fieldSizeY - tileSizeY)) {
        for (int y = 0; y <= tileSizeY; y+=tileSizeY)
            for (int x = 0; x <= fieldSizeX; x++) {
                move(line + y, x);
                printw("-");
            }
        
    }
    
    /*cells of right and left */ 
    for (int line = 0; line <= fieldSizeX - tileSizeX; line +=(fieldSizeX - tileSizeX)) {
        for (int y = 0; y <= (fieldSizeY); y+=tileSizeY)
            for (int x = 0; x <= tileSizeX; x++) {
                move(y, line + x);
                printw("-");
            }
    }

     /*cells of bottom and top lines*/
    for (int line = 0; line <= fieldSizeY - tileSizeY; line += (fieldSizeY - tileSizeY)) {
        for (int x = 0; x <= fieldSizeX; x+=tileSizeX)
                for (int y = 0; y <= tileSizeY; y++) {
                    move(y + line, x);
                    printw("|");
                }
    }
    
    move(tileSizeY, tileSizeX);
    printw("+");

    move(fieldSizeY - tileSizeY, tileSizeX);
    printw("+");

    move(tileSizeY, fieldSizeX - tileSizeX);
    printw("+");

    move(fieldSizeY - tileSizeY, fieldSizeX - tileSizeX);
    printw("+");

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
	    printGrid();
        bool flag = false;
		while (true) {
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


