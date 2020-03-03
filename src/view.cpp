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
    move(0, 140);
    printw("A");
    for (int line = 0; line <= 38; line += 38)
        for (int y = 0; y <= 4; y+=4)
            for (int x = 0; x <= 14 * 10; x++) {
                move(line + y, x);
                printw("-");
            }
            
    for (int line = 0; line <= 120; line +=120)
        for (int x = 0; x <= 20; x += 20)
            for (int y = 0; y <= 14 * 3; y++) {
                move(y, line + x);
                printw("|");
            }










    //move(45, 30);

    //printw("A");
   /* for (int y = 0; y < 2 * 4; y+=4)
        for (int x = 0; x <= 14 * 10; x++) {
            move(y, x);
            printw("-");
        }               
    for (int x = 0; x <= 14 * 10; x+=10)
        for (int y = 0; y < 2 * 5; y++) {
            move(y, x);
            printw("|");
        }*/
    //move(30, 30);
    //printw("A");
        
    /*for (int x = 0; x <= FIELD_SIZE * 8; x+=8)
        for(int y = 0; y <= FIELD_SIZE * 4; y++) {
            move(y, x);
            printw("|");
        }

    for (int y = 0; y <= FIELD_SIZE * 4; y+=4)
        for (int x = 0; x <= FIELD_SIZE * 8; x++) { 
            move(y, x);
            printw("-");
        }
    
    for (int y = 0; y <= FIELD_SIZE * 4; y+=4)
        for (int x = 0; x <= FIELD_SIZE * 8; x+=8) {
            move(y, x);
            printw("+");
        }*/
    
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