#include <ncurses.h>
#include <string>
#include "view.h"

/* Menu view */
MenuView::MenuView() {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
}

MenuView::~MenuView() {
    endwin();
}
void MenuView::MenuInteraction() {
    getmaxyx(stdscr, menuSizeY, menuSizeX);
    char list[NUMBER_OF_BUTTONS][15] = { "Add player", "Start game", "Exit" };

    char item[15];
    int ch, i;
    menuSizeX--, menuSizeY--;
    w = newwin( menuSizeY, menuSizeX, 1, 1 );
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
                return;
    }
 
    delwin(w);
}

/* Board view */
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
    clear();
    getmaxyx(stdscr, maxY, maxX);
    maxX--;
    maxY--;
    changeFieldSize();
    

    /* right and left lines*/
    for (int line = 0; line <= FIELD_SIZE_X - TILE_SIZE_X; line +=(FIELD_SIZE_X - TILE_SIZE_X)) {
        for (int x = 0; x <= TILE_SIZE_X; x += ( TILE_SIZE_X))
            for (int y = 0; y <= FIELD_SIZE_Y; y++) {
                move(y, line + x);
                addch(ACS_VLINE);
            }
    }

    /*bottom and top lines*/
    for (int line = 0; line <= FIELD_SIZE_Y - TILE_SIZE_Y; line += (FIELD_SIZE_Y - TILE_SIZE_Y)) {
        for (int y = 0; y <= TILE_SIZE_Y; y+=TILE_SIZE_Y)
            for (int x = 1; x < FIELD_SIZE_X; x++) {
                move(line + y, x);
                addch(ACS_HLINE);
            }
        
    }

    /*cells of right and left */ 
    for (int line = 0; line <= FIELD_SIZE_X - TILE_SIZE_X; line +=(FIELD_SIZE_X - TILE_SIZE_X)) {
        for (int y = 0; y <= (FIELD_SIZE_Y); y+=TILE_SIZE_Y)
            for (int x = 1; x < TILE_SIZE_X; x++) {
                move(y, line + x);
                addch(ACS_HLINE);
            }
    }

     /*cells of bottom and top lines*/
    for (int line = 0; line <= FIELD_SIZE_Y - TILE_SIZE_Y; line += (FIELD_SIZE_Y - TILE_SIZE_Y)) {
        for (int x = 0; x <= FIELD_SIZE_X; x+=TILE_SIZE_X)
                for (int y = 1; y < TILE_SIZE_Y; y++) {
                    move(y + line, x);
                    addch(ACS_VLINE);
                }
    }
   
    /* corners */
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, FIELD_SIZE_X, ACS_URCORNER);
    mvaddch(FIELD_SIZE_Y, 0, ACS_LLCORNER);
    mvaddch(FIELD_SIZE_Y, FIELD_SIZE_X, ACS_LRCORNER);

    /* |- */
    for (int x = 0; x <= FIELD_SIZE_X - TILE_SIZE_X; x+=(FIELD_SIZE_X - TILE_SIZE_X))
        for (int y = TILE_SIZE_Y; y < FIELD_SIZE_Y; y+=TILE_SIZE_Y)
            mvaddch(y, x, ACS_LTEE);

    /* -| */
    for (int x = TILE_SIZE_X; x <= FIELD_SIZE_X; x+=(FIELD_SIZE_X - TILE_SIZE_X))
        for (int y = TILE_SIZE_Y; y < FIELD_SIZE_Y; y+= TILE_SIZE_Y)
            mvaddch(y, x, ACS_RTEE);

   /*
    * _
    * |
    */
    for (int y = 0; y<= FIELD_SIZE_Y - TILE_SIZE_Y; y+=(FIELD_SIZE_Y - TILE_SIZE_Y))
        for (int x = TILE_SIZE_X; x < FIELD_SIZE_X; x+=TILE_SIZE_X)
            mvaddch(y, x, ACS_TTEE);

   /*
    * |
    * _
    */
    for (int y = TILE_SIZE_Y; y<= FIELD_SIZE_Y; y+=(FIELD_SIZE_Y - TILE_SIZE_Y))
        for (int x = TILE_SIZE_X; x < FIELD_SIZE_X; x+=TILE_SIZE_X)
            mvaddch(y, x, ACS_BTEE);


    /* crosses */
    for (int y = TILE_SIZE_Y; y <= FIELD_SIZE_Y - TILE_SIZE_Y; y += FIELD_SIZE_Y - 2 * TILE_SIZE_Y)
        for (int x = TILE_SIZE_X; x <= FIELD_SIZE_X - TILE_SIZE_X; x += FIELD_SIZE_X - 2 * TILE_SIZE_X) {
            move(y, x);
            addch(ACS_PLUS);
        }    
    
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