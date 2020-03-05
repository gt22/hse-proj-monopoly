#include <ncurses.h>
#include <string>
#include "view.h"

NcursesView::NcursesView() {
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
    FIELD_SIZE_X = maxX;
    FIELD_SIZE_Y = maxY;
    TILE_SIZE_Y = maxY / TILES_PER_LINE;
    TILE_SIZE_X = maxX / TILES_PER_LINE;
}

void NcursesView::printGrid() {
    getmaxyx(stdscr, maxY, maxX);
    maxX--;
    maxY--;
    changeFieldSize();
    

    /* right and left lines*/
    for (int line = 0; line <= FIELD_SIZE_X - TILE_SIZE_X; line +=(FIELD_SIZE_X - TILE_SIZE_X)) {
        for (int x = 0; x <= TILE_SIZE_X; x += ( TILE_SIZE_X))
            for (int y = 0; y <= FIELD_SIZE_Y; y++) {
                move(y, line + x);
                printw("|");
            }
    }

    /*bottom and top lines*/
    for (int line = 0; line <= FIELD_SIZE_Y - TILE_SIZE_Y; line += (FIELD_SIZE_Y - TILE_SIZE_Y)) {
        for (int y = 0; y <= TILE_SIZE_Y; y+=TILE_SIZE_Y)
            for (int x = 0; x <= FIELD_SIZE_X; x++) {
                move(line + y, x);
                printw("-");
            }
        
    }
    
    /*cells of right and left */ 
    for (int line = 0; line <= FIELD_SIZE_X - TILE_SIZE_X; line +=(FIELD_SIZE_X - TILE_SIZE_X)) {
        for (int y = 0; y <= (FIELD_SIZE_Y); y+=TILE_SIZE_Y)
            for (int x = 0; x <= TILE_SIZE_X; x++) {
                move(y, line + x);
                printw("-");
            }
    }

     /*cells of bottom and top lines*/
    for (int line = 0; line <= FIELD_SIZE_Y - TILE_SIZE_Y; line += (FIELD_SIZE_Y - TILE_SIZE_Y)) {
        for (int x = 0; x <= FIELD_SIZE_X; x+=TILE_SIZE_X)
                for (int y = 0; y <= TILE_SIZE_Y; y++) {
                    move(y + line, x);
                    printw("|");
                }
    }
    
    move(TILE_SIZE_Y, TILE_SIZE_X);
    printw("+");

    move(FIELD_SIZE_Y - TILE_SIZE_Y, TILE_SIZE_X);
    printw("+");

    move(TILE_SIZE_Y, FIELD_SIZE_X - TILE_SIZE_X);
    printw("+");

    move(FIELD_SIZE_Y - TILE_SIZE_Y, FIELD_SIZE_X - TILE_SIZE_X);
    printw("+");

    move(FIELD_SIZE_Y, FIELD_SIZE_X);
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