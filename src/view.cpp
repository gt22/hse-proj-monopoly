#include <ncurses.h>
#include "view.h"

NcursesView::NcursesView() {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
}

NcursesView::~NcursesView() {
    endwin();
}

void NcursesView::printGrid() {
    /*bottom and top lines*/
    for (int line = 0; line <= FIELD_SIZE_Y - TILE_SIZE_Y; line += (FIELD_SIZE_Y - TILE_SIZE_Y)) {
        for (int y = 0; y <= TILE_SIZE_Y; y+=TILE_SIZE_Y)
            for (int x = 0; x <= FIELD_SIZE_X; x++) {
                move(line + y, x);
                printw("-");
            }
        for (int x = 3 * TILE_SIZE_X; x <= FIELD_SIZE_X - 2 * TILE_SIZE_X; x+=TILE_SIZE_X)
            for (int y = 0; y <= TILE_SIZE_Y; y++) {
                move(y + line, x);
                printw("|");
            }
    }
    /* right and left lines*/
    for (int line = 0; line <= FIELD_SIZE_X - 2 * TILE_SIZE_X; line +=(FIELD_SIZE_X - 2 * TILE_SIZE_X)) {
        for (int x = 0; x <= 2 * TILE_SIZE_X; x += (2 * TILE_SIZE_X))
            for (int y = 0; y <= FIELD_SIZE_Y; y++) {
                move(y, line + x);
                printw("|");
            }

        for (int y = 2 * TILE_SIZE_Y; y <= (FIELD_SIZE_Y - TILE_SIZE_Y); y+=TILE_SIZE_Y)
            for (int x = 0; x <= 2 * TILE_SIZE_X; x++) {
                move(y, line + x);
                printw("-");
            }

    }
  
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