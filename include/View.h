#ifndef _VIEW_H
#define _VIEW_H

#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include <string>
#include <ncurses.h>

class AbstractView {
public:
    explicit AbstractView(Manager& manager);
    virtual ~AbstractView() = default;
protected:
    Manager& manager;
};


class MonopolyView : public AbstractView {
public:
    explicit MonopolyView(Manager& manager);
   // virtual void drawField();
   // virtual void printStatus();

   virtual void redraw() = 0;
   virtual PlayerReply processRequest(Player& p, PlayerRequest req) = 0;
   virtual void processMessage(Player& p, PlayerMessage mes) = 0;
  // virtual void getCoord(int &x, int &y, int index);

   // virtual Input::InputAction getAction(); //TODO: как православно обработать экшены
};

class NcursesView final : public MonopolyView {
public:
    explicit NcursesView(Manager& manager);
    ~NcursesView() override;
    bool getActionAlpha();
    void printGrid();
    void runGame();
    void getCoord(int &x, int &y, int index);
    void drawPlayer(int x, int y, int color);
    void redraw() override;
    //void drawField() override;
    //void printStatus() override;
    PlayerReply processRequest(Player& p, PlayerRequest req) override;
    void processMessage(Player& p, PlayerMessage mes) override;
    void changeFieldSize();

    int transformCoord(int x, int y);
    int mouseAction();
private:
    const static int TILES_PER_LINE = 11;
    int maxX = 0;
    int maxY = 0;
    int fieldSizeX = 0;
    int fieldSizeY = 0;
    int tileSizeX = 0;
    int tileSizeY = 0;
    int ar[6] ={0, 0, 1, 5, 7, 15};
};
#endif