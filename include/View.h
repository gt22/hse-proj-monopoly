#ifndef _VIEW_H
#define _VIEW_H

#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include <string>

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

   virtual void redraw(const Board& board) = 0;
   virtual PlayerReply processRequest(Player& p, PlayerRequest req) = 0;
   virtual void processMessage(Player& p, PlayerMessage mes) = 0;
};

class TerminalView : public MonopolyView {
public:
    explicit TerminalView(Manager& manager);

    virtual void redraw(const Board& board);
    virtual PlayerReply processRequest(Player& p, PlayerRequest req) ;
    virtual void processMessage(Player& p, PlayerMessage mes) ;
};
#endif