#include "ncursesw/ncurses.h"
#include <string>
#include "Player.h"
#include "View.h"
#include "Manager.h"
#include "Board.h"
#include "Field.h"
#include <memory>

AbstractView::AbstractView(Manager& manager) : manager(manager) {}

MonopolyView::MonopolyView(Manager &manager) : AbstractView(manager) {}

NcursesView::NcursesView(Manager& manager) : MonopolyView(manager) {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    //cbreak();
    mousemask(ALL_MOUSE_EVENTS, NULL);
    curs_set(0);
}

NcursesView::~NcursesView() {
    endwin();
}

// PLAYER
void NcursesView::getCoord(int &x, int &y, int index) {
    if (index >= 40)
        index %= 40;
    if (index < 0)
        index +=40;
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
    init_pair(0, COLOR_RED, COLOR_BLACK);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(color));
    mvaddch(y, x, ' '|A_REVERSE);
    attroff(COLOR_PAIR(color));
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
    //int ind = 0;
    int pos;
    while (true) {
        bool flag = false;
        MEVENT event;
        printGrid();
       // redraw();
        //int x = 0, y = 0;
        int enteredChar = getch();
        refresh();
        switch(enteredChar) {
            case 'x':
                flag = true;
                clear();
                break;
            case KEY_MOUSE:
                if (getmouse(&event) == OK) {
                    if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
                        pos = transformCoord(event.x, event.y);
                        mvprintw(12, 50, "You chose %d", pos);
                        //if (pos != -1)
                         //   break;
                    } else if (event.bstate & BUTTON1_CLICKED) {
                        pos = transformCoord(event.x, event.y);
                        //mvprintw(10, 50, "You chose %d", pos);
                        if (pos != -1)
                            writeTileInfo(pos);
                    }
                }
                break;
            case 'r':
                processRequestTest();
                //processRequest(...);
                break;
        }
        refresh();
        if (flag)
            break;
    }
}

PlayerReply NcursesView::processRequest(Player& player, PlayerRequest req) {
    WINDOW* actionWindow = newwin(fieldSizeY - 2 * tileSizeY, fieldSizeX - 2 * tileSizeX, tileSizeY + 1, tileSizeX + 1);
    box(actionWindow, 0, 0);
    wrefresh(actionWindow);
    std::size_t x = tileSizeX + 1;
    std::size_t y = tileSizeY + 1;
    mvwprintw(actionWindow, y++, x, "Choose action number:");
    std::vector<std::string> actionNames;
    for (std::size_t i = 0; i < req.availableActions.size(); i++) {
        if (req.availableActions[i] == PlayerAction::PAY_TAX) {
            actionNames.push_back("PAY TAX");
        } else if (req.availableActions[i] == PlayerAction::BUY_PROPERTY) {
            actionNames.push_back("BUY PROPERTY");
        } else if (req.availableActions[i] == PlayerAction::BUY_BUILDING) {
            actionNames.push_back("BUY BUILDING");
        } else if (req.availableActions[i] == PlayerAction::BUY_HOTEL) {
            actionNames.push_back("BUY HOTEL");
        } else if (req.availableActions[i] == PlayerAction::USE_CARD) {
            actionNames.push_back("USE CARD");
        } else if (req.availableActions[i] == PlayerAction::PAY_TO_OTHER_PLAYER) {
            actionNames.push_back("PAY TO OTHER PLAYER");
        } else if (req.availableActions[i] == PlayerAction::TAKE_CARD) {
            actionNames.push_back("TAKE CARD");
        } else if (req.availableActions[i] == PlayerAction::MORTGAGE_HOLDINGS) {
            actionNames.push_back("MORTGAGE HOLDINGS");
        } else if (req.availableActions[i] == PlayerAction::START_TRADE) {
            actionNames.push_back("START TRADE");
        } else if (req.availableActions[i] == PlayerAction::START_TRADE_NEW_FIELD) {
            actionNames.push_back("START TRADE NEW FIELD");
        } else if (req.availableActions[i] == PlayerAction::ROLL_DICE) {
            actionNames.push_back("ROLL DICE");
        } else if (req.availableActions[i] == PlayerAction::END_TURN) {
            actionNames.push_back("END TURN");
        }
    }

    std::size_t i;
    for (i = 0; i < req.availableActions.size(); i++) {
        mvwprintw(actionWindow, y + i, x, "%d. %s\n", i + 1, actionNames[i].c_str());
    }
    y += i;
    nocbreak();
    echo();

    std::string actionNum;
    mvwprintw(actionWindow, y, x, "ACTION: ");
    int number;
    while (true) {
        wrefresh(actionWindow);
        wmove(actionWindow, y, x + 8);
        int ch = wgetch(actionWindow);

        while (ch != '\n') {
            actionNum.push_back(ch);
            ch = wgetch(actionWindow);
        }

        bool flag = true;
        number = 0;
        for (char c : actionNum) {
            if (c >= '0' && c <= '9')
                number = number * 10 + (c - '0');
            else {
                flag = false;
                number = 0;
                break;
            }
        }
        if (flag && number < 13)
            break;
        for (int erCh = 0; erCh < int(actionNum.size()); erCh++)
            mvwaddch(actionWindow, y, x + 8 + erCh, ' ');
        actionNum.clear();
    }
    noecho();
    cbreak();
    wrefresh(actionWindow);

    y++;
    mvwprintw(actionWindow, y, x, "You chose %d", number);
    int ch = wgetch(actionWindow);

    while (ch != 'x') {
        ch = wgetch(actionWindow);
    }
    wborder(actionWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(actionWindow);
    delwin(actionWindow);

    if (actionNames[number - 1] == "PAY TAX") {
        return std::make_unique<PayTaxReply>();
    } else if (actionNames[number - 1] == "BUY PROPERTY") {
        return std::make_unique<BuyPropertyReply>();
    } else if (actionNames[number - 1] == "BUY BUILDING") {
        return std::make_unique<BuyBuildingReply>();
    } else if (actionNames[number - 1] == "BUY HOTEL") {
       // return std::make_unique<BuyHotelReply>();
    } else if (actionNames[number - 1] == "USE CARD") {
        //return std::make_unique<UseCardReply>();
    } else if (actionNames[number - 1] == "PAY TO OTHER PLAYER") {
        // return std::make_unique<PayToOtherPlayerReply>();
    } else if (actionNames[number - 1] == "TAKE CARD") {
        //return std::make_unique<TakeCardReply>();
    } else if (actionNames[number - 1] == "MORTGAGE HOLDINGS") {
        // return std::make_unique<MortageHoldingsReply>();
    } else if (actionNames[number - 1] == "START TRADE") {
        //return std::make_unique<StartTradeReply>();
    } else if (actionNames[number - 1] == "START TRADE NEW FIELD") {
        // return std::make_unique<StartTradeNewFieldReply>();
    } else if (actionNames[number - 1] == "ROLL_DICE") {
        //return std::make_unique<RollDiceReply>();
    }
    return std::make_unique<EndTurnReply>();
}

void NcursesView::processRequestTest() {
    std::vector<PlayerAction> availableActions;
    for(int i = 0; i < 12; i++){
        availableActions.push_back(static_cast<PlayerAction>(i));
    }


    WINDOW* actionWindow = newwin(fieldSizeY - 2 * tileSizeY - 1, fieldSizeX - 2 * tileSizeX - 1, tileSizeY + 1, tileSizeX + 1);
    box(actionWindow, 0, 0);
    wrefresh(actionWindow);
    std::size_t x = tileSizeX + 1;
    std::size_t y = tileSizeY + 1;
    mvwprintw(actionWindow, y++, x, "Choose action number:");
    std::vector<std::string> actionNames;
    for (std::size_t i = 0; i < availableActions.size(); i++) {
        if (availableActions[i] == PlayerAction::PAY_TAX) {
            actionNames.push_back("PAY TAX");
        } else if (availableActions[i] == PlayerAction::BUY_PROPERTY) {
            actionNames.push_back("BUY PROPERTY");
        } else if (availableActions[i] == PlayerAction::BUY_BUILDING) {
            actionNames.push_back("BUY BUILDING");
        } else if (availableActions[i] == PlayerAction::BUY_HOTEL) {
            actionNames.push_back("BUY HOTEL");
        } else if (availableActions[i] == PlayerAction::USE_CARD) {
            actionNames.push_back("USE CARD");
        } else if (availableActions[i] == PlayerAction::PAY_TO_OTHER_PLAYER) {
            actionNames.push_back("PAY TO OTHER PLAYER");
        } else if (availableActions[i] == PlayerAction::TAKE_CARD) {
            actionNames.push_back("TAKE CARD");
        } else if (availableActions[i] == PlayerAction::MORTGAGE_HOLDINGS) {
            actionNames.push_back("MORTGAGE HOLDINGS");
        } else if (availableActions[i] == PlayerAction::START_TRADE) {
            actionNames.push_back("START TRADE");
        } else if (availableActions[i] == PlayerAction::START_TRADE_NEW_FIELD) {
            actionNames.push_back("START TRADE NEW FIELD");
        } else if (availableActions[i] == PlayerAction::ROLL_DICE) {
            actionNames.push_back("ROLL DICE");
        } else if (availableActions[i] == PlayerAction::END_TURN) {
            actionNames.push_back("END TURN");
        }
    }

    std::size_t i;
    for (i = 0; i < availableActions.size(); i++) {
        mvwprintw(actionWindow, y + i, x, "%d. %s\n", i + 1, actionNames[i].c_str());
    }
    y += i;
    nocbreak();
    echo();

    std::string actionNum;
    mvwprintw(actionWindow, y, x, "ACTION: ");
    int number;
    while (true) {
        wrefresh(actionWindow);
        wmove(actionWindow, y, x + 8);
        int ch = wgetch(actionWindow);

        while (ch != '\n') {
            actionNum.push_back(ch);
            ch = wgetch(actionWindow);
        }

        bool flag = true;
        number = 0;
        for (char c : actionNum) {
            if (c >= '0' && c <= '9')
                number = number * 10 + (c - '0');
            else {
                flag = false;
                number = 0;
                break;
            }
        }
        if (flag && number < 13)
            break;
        for (int erCh = 0; erCh < int(actionNum.size()); erCh++)
            mvwaddch(actionWindow, y, x + 8 + erCh, ' ');
        actionNum.clear();
    }
    noecho();
    cbreak();
    wrefresh(actionWindow);

    y++;
    mvwprintw(actionWindow, y, x, "You chose %d", number);
    int ch = wgetch(actionWindow);

    while (ch != 'x') {
        ch = wgetch(actionWindow);
    }
    wborder(actionWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(actionWindow);
    delwin(actionWindow);
    return;
}


void NcursesView::processMessage(Player &p, PlayerMessage mes) {
    diceValue = mes.message;
    std::make_unique<EndTurnReply>();
}

void NcursesView::redraw(const Board& board) {
    clear();
    refresh();
    printGrid();

    size_t size = board.getPlayers().size();

    for (size_t i = 0; i < size; i++) {
        int x, y;
        getCoord(x, y, board.getPlayer(board.getPlayerToken(i)).position);
        if (i == 0) {
            x-=2;
            y--;
        } else if (i == 1) {
            y--;
        } else if (i == 2) {
            x+=2;
            y--;
        } else if (i == 3) {
            x-=2;
            y++;
        } else if (i == 4) {
            y++;
        } else if (i == 5) {
            x+=2;
            y++;
        }
        drawPlayer(x, y, i);
    }
}

int NcursesView::transformCoord(int x, int y) {
    if (x > tileSizeX && y > tileSizeY && x < fieldSizeX - tileSizeX && y < fieldSizeY - tileSizeY)
        return -1;
    if (y < tileSizeY)
        return x / tileSizeX;
    else if (x > fieldSizeX - tileSizeX)
        return TILES_PER_LINE - 1 + y / tileSizeY;
    else if (y > fieldSizeY - tileSizeY)
        return 3 * TILES_PER_LINE -  3 - x / tileSizeX;
    else if (x < tileSizeX)
        return 4 * TILES_PER_LINE - 4 - y / tileSizeY;
    return -1;
}

void NcursesView::writeTileInfo(int pos) {
    WINDOW* cardWindow = newwin(tileSizeY * 4, tileSizeX * 3 / 2, fieldSizeY / 2 - 2 * tileSizeY,  fieldSizeX / 2 - tileSizeX * 3 / 4);
    box(cardWindow, 0, 0);
    wrefresh(cardWindow);
    int x = tileSizeX / 2;
    int y = 1;

    mvwprintw(cardWindow, y, x, "CARD #%i", pos + 1);
    y++;
    if (pos == 0) {
        y = tileSizeY * 2 - 1;
        x+=3;
        mvwprintw(cardWindow, y++, x, "GO");
        mvwprintw(cardWindow, y, x, "->");
        x-=3;
    } else if (pos == 10) {
        for (int yi = y; yi < tileSizeY * 4 - 1; yi++)
            for (int xi = 0; xi < tileSizeX * 3 / 2; xi+=3) {
                mvwaddch(cardWindow, yi, xi, ACS_VLINE);
            }
        y = tileSizeY * 2 - 1;
        x+=1;
        mvwprintw(cardWindow, y++, x, "IN JAIL");
        x-=2;
        mvwprintw(cardWindow, y, x, "OR VISITING");
    } else if (pos == 20) {
        y = tileSizeY * 2 - 1;
        x+=2;
        mvwprintw(cardWindow, y++, x, "FREE");
        x-=2;
        mvwprintw(cardWindow, y, x, "PARKING");
    } else if (pos == 30) {
        y = tileSizeY * 2 - 1;
        mvwprintw(cardWindow, y++, x, "GO TO JAIL");
    }
    /*
    const Board& board = manager.sendBoard();
    std::string name = board.field[pos]->name;
    std::vector<std::string> info = board.field[pos]->writeTileInfo();
    y = tileSizeY * 2 - 1;
    for (size_t i = 0; i < info.size(); i++) {
        mvwprintw(cardWindow, y, x, "%s", info[i].c_str());
    }
     */


    // STREET:
    // NAME
    // PRICE
    // 1,2,3,4 HOUSE PRICE
    // HOTEL
    // OWNER?


    wrefresh(cardWindow);
    int ch = wgetch(cardWindow);

    while (ch != 'x') {
        ch = wgetch(cardWindow);
    }
    wborder(cardWindow, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(cardWindow);
    delwin(cardWindow);
    return;
}
