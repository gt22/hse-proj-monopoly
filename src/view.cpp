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
    for (int line = 0; line <= 40; line += 40) {
        for (int y = 0; y <= 4; y+=4)
            for (int x = 0; x <= 14 * 10; x++) {
                move(line + y, x);
                printw("-");
            }
        for (int x = 30; x <= 120; x+=10)
            for (int y = 0; y <=4; y++) {
                move(y + line, x);
                printw("|");
            }
    }
    /*right and left lines*/
    for (int line = 0; line <= 120; line +=120) {
        for (int x = 0; x <= 20; x += 20)
            for (int y = 0; y <= 11 * 4; y++) {
                move(y, line + x);
                printw("|");
            }

        for (int y = 8; y <= 40; y+=4)
            for (int x = 0; x <= 20; x++) {
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