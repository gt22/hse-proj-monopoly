#include <string>
#include "Player.h"
#include "View.h"
#include "Manager.h"
#include "Board.h"
#include "Field.h"
#include <memory>
#include <iostream>

AbstractView::AbstractView(Manager& manager) : manager(manager) {}

MonopolyView::MonopolyView(Manager &manager) : AbstractView(manager) {}

TerminalView::TerminalView(Manager& manager) : MonopolyView(manager) {
    std::cout << "===== Monopoly =====" << std::endl;
}

void printPlayer(const PlayerData& p) {
    auto& o = std::cout;
    o << "========" << std::endl;
    o << "Player: " << p.name << ", token " << (int) p.token << ": " << std::endl;
    o << "Pos: " << p.position << std::endl;
    o << "========" << std::endl;

}

void TerminalView::redraw(const Board &board) {
    for(const auto& p : board.getPlayers()) {
        printPlayer(p);
    }
}

void TerminalView::processMessage(Player &p, PlayerMessage mes) {
    std::cout << "Message for player " << (int) p.token << ": " << mes.message << std::endl;
}

PlayerReply TerminalView::processRequest(Player &p, PlayerRequest req) {
    std::cout << "Request for player" << (int) p.token << ": " << std::endl;
    std::cout << "Msg: " << req.message << std::endl;
    for(size_t i = 0; i < req.availableActions.size(); i++) {
        PlayerAction a = req.availableActions[i];
        std::cout << (i + 1) << ": ";
        switch(a) {

            case PlayerAction::PAY_TAX:
                std::cout << "PAY_TAX"; break;
            case PlayerAction::BUY_PROPERTY:
                std::cout << "BUY_PROPERTY"; break;
            case PlayerAction::BUY_BUILDING:
                std::cout << "BUY_BUILDING"; break;
            case PlayerAction::BUY_HOTEL:
                std::cout << "BUY_HOTEL"; break;
            case PlayerAction::USE_CARD:
                std::cout << "USE_CARD"; break;
            case PlayerAction::PAY_TO_OTHER_PLAYER:
                std::cout << "PAY_TO_OTHER_PLAYER"; break;
            case PlayerAction::TAKE_CARD:
                std::cout << "TAKE_CARD"; break;
            case PlayerAction::MORTGAGE_HOLDINGS:
                std::cout << "MORTGAGE_HOLDINGS"; break;
            case PlayerAction::START_TRADE:
                std::cout << "START_TRADE"; break;
            case PlayerAction::START_TRADE_NEW_FIELD:
                std::cout << "START_TRADE_NEW_FIELD"; break;
            case PlayerAction::ROLL_DICE:
                std::cout << "ROLL_DICE"; break;
            case PlayerAction::END_TURN:
                std::cout << "END_TURN"; break;
            case PlayerAction::EXIT_GAME:
                std::cout << "EXIT_GAME"; break;
            case PlayerAction::LOSE:
                std::cout << "LOSE"; break;
            case PlayerAction::BUY_BACK_PROPERTY:
                std::cout << "BUY BACK PROPERTY"; break;
        }
        std::cout << std::endl;
    }
    int ans;
    std::cin >> ans;
    return std::make_unique<PlayerReplyData>(req.availableActions[ans - 1]);
}






