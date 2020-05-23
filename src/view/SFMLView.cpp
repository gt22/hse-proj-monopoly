#include "view/SFMLView.h"
#include "Manager.h"
#include "Field.h"
#include <view/VectorOps.h>
#include <iostream>
#include <cassert>

using Vec::transpose;

sf::Color getColor(Token tok) {
    switch (tok) {
        case Token::DOG:
            return sf::Color::White;// ORANGE
        case Token::HAT:
            return sf::Color::Red; // PINK
        case Token::BOOT:
            return sf::Color::Blue; // BLUE
        case Token::CAT:
            return sf::Color::Cyan; // CYAN
        case Token::CAR:
            return sf::Color::Magenta; // PURPLE
        case Token::SHIP:
            return sf::Color::Green; // GREEN
        case Token::FREE_FIELD:
            return sf::Color::Transparent;
    }
    return sf::Color::Transparent;
}

sf::Color getColor(Color color) {
    switch(color) {

        case Color::COL1:
            return sf::Color(105, 50, 50);
        case Color::COL2:
            return sf::Color(36, 194, 181);
        case Color::COL3:
            return sf::Color(213, 0, 112);
        case Color::COL4:
            return sf::Color(255, 135, 0);
        case Color::COL5:
            return sf::Color(182, 0, 0);
        case Color::COL6:
            return sf::Color(175, 165, 15);
        case Color::COL7:
            return sf::Color(30, 175, 15);
        case Color::COL8:
            return sf::Color(15, 109, 175);
        case Color::NO_COL:
            return sf::Color::Transparent;
    }
    return sf::Color::Transparent;
}

std::string SFMLView::tokenToString(Token token) {
    if (token == Token::SHIP)
        return "SHIP";
    else if (token == Token::BOOT)
        return "BOOT";
    else if (token == Token::CAR)
        return "CAR";
    else if (token == Token::CAT)
        return "CAT";
    else if (token == Token::DOG)
        return "DOG";
    else if (token == Token::HAT)
        return "HAT";
    throw std::invalid_argument("no such token");
}

SFMLView::SFMLView(Manager &manager) : manager(manager) {
    window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 20 / 21, sf::VideoMode::getDesktopMode().height * 9 / 10),
            "Monopoly", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);
    mainFont.loadFromFile("Ubuntu-R.ttf");

    enteredT.setFont(mainFont);
    enteredT.setCharacterSize(15);
    enteredT.setFillColor(sf::Color::White);
    box.setFont(mainFont);
    box.setCharacterSize(15);
    box.setFillColor(sf::Color::White);
    message.setFont(mainFont);
    message.setCharacterSize(20);
    message.setFillColor(sf::Color::White);


    addActionButton(PlayerAction::END_TURN,
                    "images/END_TURN.png",
                    "By pressing this button you will finish your turn",
                    makeReplyGenerator<EndTurnReply>());
    addActionButton(PlayerAction::ROLL_DICE,
                    "images/ROLL_DICE.png",
                    "By pressing this button you will roll dice",
                    makeReplyGenerator<RollDiceReply>());
    addActionButton(PlayerAction::PAY_TAX,
                    "images/PAY_TAX.png",
                    "By pressing this button you will pay tax to bank",
                    makeReplyGenerator<PayTaxReply>());
    addActionButton(PlayerAction::PAY_TO_OTHER_PLAYER,
                    "images/PAY_TO_OTHER_PLAYER.png",
                    "By pressing this button you will pay money to other player",
                    makeReplyGenerator<PayToOtherPlayersReply>());
    addActionButton(PlayerAction::BUY_FIELD,
                    "images/BUY_FIELD.png",
                    "By pressing this button you will buy this field",
                    makeReplyGenerator<BuyPropertyReply>());
    addActionButton(PlayerAction::SELL_FIELD,
                    "images/SELL_FIELD.png",
                    "By pressing this button you will sell this field",
                    makeReplyGenerator<SellPropertyReply>());
    addActionButton(PlayerAction::BUY_HOUSE,
                    "images/BUY_HOUSE.png",
                    "By pressing this button you will buy house",
                    makeReplyGenerator<BuyBuildingReply>());
    addActionButton(PlayerAction::SELL_HOUSE,
                    "images/SELL_HOUSE.png",
                    "By pressing this button you will sell house",
                    makeReplyGenerator<SellBuildingReply>());
    addActionButton(PlayerAction::BUY_HOTEL,
                    "images/BUY_HOTEL.png",
                    "By pressing this button you will buy hotel",
                    makeReplyGenerator<BuyHotelReply>());
    addActionButton(PlayerAction::SELL_HOTEL,
                    "images/SELL_HOTEL.png",
                    "By pressing this button you will sell hotel",
                    makeReplyGenerator<SellHotelReply>());
    addActionButton(PlayerAction::TAKE_CARD,
                    "images/TAKE_CARD.png",
                    "By pressing this button you will take card",
                    makeReplyGenerator<TakeCardReply>());
    addActionButton(PlayerAction::USE_CARD,
                    "images/USE_CARD.png",
                    "By pressing this button you will use card",
                    makeReplyGenerator<UseCardReply>());
    addActionButton(PlayerAction::START_TRADE_NEW_FIELD,
                    "images/START_TRADE_NEW_FIELD.png",
                    "By pressing this button you won't buy this field and start auction between other players",
                    makeReplyGenerator<StartTradeNewFieldReply>());
    addActionButton(PlayerAction::START_TRADE,
                    "images/START_TRADE.png",
                    "By pressing this button you will start trade with another player",
                    makeReplyGenerator<StartTradeReply>());
    addActionButton(PlayerAction::MORTGAGE_HOLDINGS,
                    "images/MORTGAGE_HOLDINGS.png",
                    "By pressing this button you will get mortgage",
                    makeReplyGenerator<MortgageHoldingsReply>());
    addActionButton(PlayerAction::BUY_BACK_PROPERTY,
                    "images/BUY_BACK_PROPERTY.png",
                    "By pressing this button you will buy your field back",
                    makeReplyGenerator<BuyBackProperty>());
    addActionButton(PlayerAction::FINISH_GAME,
                    "images/EXIT_GAME.png",
                    "By pressing this button you will forcely end the game for all players",
                    makeReplyGenerator<FinishGameReply>());
    addActionButton(PlayerAction::EXIT_GAME,
                    "images/FINISH_GAME.png",
                    "By pressing this button you will leave the game",
                    makeReplyGenerator<ExitGameReply>());



    tokenButtons[Token::DOG].setTexture("images/DOG.png");
    tokenButtons[Token::BOOT].setTexture("images/BOOT.png");
    tokenButtons[Token::CAR].setTexture("images/CAR.png");
    tokenButtons[Token::CAT].setTexture("images/CAT.png");
    tokenButtons[Token::HAT].setTexture("images/HAT.png");
    tokenButtons[Token::SHIP].setTexture("images/SHIP.png");

    events.addHandler<sf::Event::Closed>([this]() { window.close(); });
    events.addHandler<sf::Event::Resized>([this](sf::Event::SizeEvent e) {
        window.setView(sf::View(sf::FloatRect(0, 0, e.width, e.height)));
        onResize(e);
    });
    events.addHandler<sf::Event::MouseMoved>([this](sf::Event::MouseMoveEvent e) {
        if (!isMenu) {
            box.setString(std::string(this->tooltips.getTooltip(e)));
            box.setPosition(sf::Vector2f(20, window.getSize().y - box.getLocalBounds().height * 2));
        }
    });
    events.addHandler<sf::Event::TextEntered>([this](sf::Event::TextEvent e) {
        if (isSumReq) {
            if (e.unicode == 8 && !enteredText.empty()){
                enteredText.pop_back();
            } else if (e.unicode < 128) {
                    if (std::isdigit((char)e.unicode)) {
                        enteredText.push_back((char) e.unicode);
                    }
            }
        }
    });
    events.addHandler<sf::Event::KeyPressed>([this](sf::Event::KeyEvent e) {
        if (isSumReq) {
            if (e.code == sf::Keyboard::Return) {
                if (!enteredText.empty()) {
                    isSumReq = false;
                    makeSumReply(std::make_unique<SumReplyData>(std::stoi(enteredText)));
                }
            }
        }
    });


    onResize({window.getSize().x, window.getSize().y});

    events.addHandler<sf::Event::MouseButtonPressed>([this](sf::Event::MouseButtonEvent e) {
        if (!isMenu) {
            buttons.handle(e);
            if (e.button == sf::Mouse::Left) {
                if (isNumReq) {
                    for (std::size_t i = 0; i < fieldButtons.fieldButtons.size(); i++) {
                        if (fieldButtons.fieldButtons[i].isValidTarget(e)) {
                            isNumReq = false;
                            makeNumReply(std::make_unique<NumReplyData>(i));
                            break;
                        }
                    }
                } else if (isTradeReq) {
                    if (participateInTrade[0].isMouseOver(window)) {
                        isTradeReq = false;
                        makeTradeReply(std::make_unique<PlayerTradeReplyData>(PlayerTradeAction::PARTICIPATE));
                    } else if (participateInTrade[1].isMouseOver(window)) {
                        isTradeReq = false;
                        makeTradeReply(std::make_unique<PlayerTradeReplyData>(PlayerTradeAction::REFUSE));
                    }
                } else if (isTokenReq) {
                    for (auto& t: tokenButtons) {
                        if (t.second.isMouseOver(window)) {
                            isTokenReq = false;
                            makeTokenReply(std::make_unique<TokenReplyData>(t.first));
                        }
                    }
                }
            } else if (e.button == sf::Mouse::Right) {
                for (std::size_t i = 0; i < fieldButtons.fieldButtons.size(); i++) {
                    if (fieldButtons.fieldButtons[i].isValidTarget(e)) {
                        curCardIndex = i;
                        break;
                    }
                }
            }
        }
        if (isMenu) {
            if (e.button == sf::Mouse::Left) {
                if (numOfAddedPlayers >= 2 && menuButtons[0].isMouseOver(window)) {
                    isMenu = false;
                    this->manager.startGame();
                } else if (numOfAddedPlayers < 6 && menuButtons[1].isMouseOver(window)) {
                    isTokenDraw = true;
                } else if (menuButtons[2].isMouseOver(window)) {

                } else if (menuButtons[3].isMouseOver(window)) {
                    dispose();
                }

                if (isTokenDraw) {
                    for (auto &t : tokenButtons) {
                        if (t.second.getClickability() && t.second.isMouseOver(window)) {
                            std::string name = "Player" + std::to_string(numOfAddedPlayers + 1);
                            this->manager.addPlayer(std::make_unique<LocalPlayer>(t.first, name, *this));
                            t.second.deactivate();
                            isTokenDraw = false;
                            ++numOfAddedPlayers;
                        }
                    }
                }

            }
        }
        });

    //TODO: temp
}

void SFMLView::dispose() {
    windowMutex.lock();
    shouldClose = true;
    windowMutex.unlock();
    requestMutex.lock();
    curReply = std::make_unique<ExitGameReply>();
    requestCond.notify_all();
    requestMutex.unlock();
}


void SFMLView::mainLoop() {
    while (window.isOpen()) {
        {
            std::lock_guard m(windowMutex);
            if (shouldClose) {
                window.close();
                break;
            }
        }
        handleRequest();
        sf::Event e{};
        while (window.pollEvent(e)) {
            events.handleEvent(e);
        }
        window.clear();
        monopolyDrawer();
        if (isMenu) {
            menuDrawer();
        }

        window.display();
    }
}

bool doesFit(const sf::Text &t, float width) {
    return t.getLocalBounds().width < width;
}

void SFMLView::drawField(const BoardModel &board) {
    for (const auto &fieldTile : board.field) {
        int i = fieldTile.position;
        const auto &viewTile = shapes.fieldRects[i];
        bool isSide = (i / 10) % 2 == 0;
        auto[x, y] = viewTile.getPosition();
        auto[w, h] = isSide ? viewTile.getSize() : transpose(viewTile.getSize());
        const float shift = (w / 4);
        window.draw(viewTile);

        if (fieldTile.color.has_value()) {
            const float size = w / 6;
            sf::RectangleShape colorRect(sf::Vector2f(size, h));
            colorRect.setFillColor(getColor(fieldTile.color.value()));
            if(!isSide) colorRect.rotate(90);
            switch(i / 10) {
                case 0: colorRect.setPosition(viewTile.getPosition() + sf::Vector2f(w - size, 0)); break;
                case 1: colorRect.setPosition(viewTile.getPosition() + sf::Vector2f(h, w - size)); break;
                case 2: colorRect.setPosition(viewTile.getPosition()); break;
                case 3: colorRect.setPosition(viewTile.getPosition() + sf::Vector2f(h, 0)); break;
            }
            window.draw(colorRect);
        }

        if (fieldTile.owner.has_value()) {
            sf::Color color = getColor(fieldTile.owner.value());
            const float size = w / 6;
            sf::RectangleShape ownerLabel(sf::Vector2f(size, size));
            ownerLabel.setFillColor(color);
            switch(i / 10) {
                case 0: ownerLabel.setPosition(viewTile.getPosition()); break;
                case 1: ownerLabel.setPosition(viewTile.getPosition() + sf::Vector2f(h - size, 0)); break;
                case 2: ownerLabel.setPosition(viewTile.getPosition() + sf::Vector2f(w - size, h - size)); break;
                case 3: ownerLabel.setPosition(viewTile.getPosition() + sf::Vector2f(0, w - size)); break;
            }
            window.draw(ownerLabel);
            auto[ow, oh] = ownerLabel.getSize();
            if (fieldTile.isMortgaged) {
                sf::Color mortgageColor = sf::Color(150, 150, 150);
                const float mortgageSize = w / 20;
                sf::CircleShape mortgageLabel(mortgageSize);
                mortgageLabel.setOrigin(mortgageLabel.getRadius(), mortgageLabel.getRadius());
                mortgageLabel.setFillColor(mortgageColor);
                switch(i / 10) {
                    case 0: mortgageLabel.setPosition(viewTile.getPosition() + sf::Vector2f(ow / 2, oh / 2)); break;
                    case 1: mortgageLabel.setPosition(viewTile.getPosition() + sf::Vector2f(h - size, 0) + sf::Vector2f(ow / 2, oh / 2)); break;
                    case 2: mortgageLabel.setPosition(viewTile.getPosition() + sf::Vector2f(w - size, h - size) + sf::Vector2f(ow / 2, oh / 2)); break;
                    case 3: mortgageLabel.setPosition(viewTile.getPosition() + sf::Vector2f(0, w - size) + sf::Vector2f(ow / 2, oh / 2)); break;
                }
                window.draw(mortgageLabel);
            }
        }

        std::string s;
        if (fieldTile.shortName.has_value())
            s = std::string(fieldTile.shortName.value());
        else
            s = std::string(fieldTile.name);
        sf::Text name(s, mainFont);
        int fsize = 10;
        name.setCharacterSize(fsize);
        while (!doesFit(name, h * 0.7f)) {
            auto lastSpace = s.rfind(' ');
            if (lastSpace == std::string::npos) break;
            s.replace(lastSpace, 1, "\n");
            name.setString(s);
        }
        int j = 0;
        while (doesFit(name, h * 0.7f) && j++ < 10) {
            name.setCharacterSize(++fsize);
        }
        auto[nx, ny, nw, nh] = name.getLocalBounds();
        const float align = ((h - nw) / 2);
        if (i % 10 == 0) {
            name.setOrigin(nx + nw / 2, ny + nh / 2);
            name.setPosition(x + h / 2, y + w / 2);
            name.rotate(isSide ? 45 : -45);
        } else {
            switch (i / 10) {
                case 0:
                    if (i == 2 || i == 4 || i == 5 || i == 7) {
                        name.rotate(+90);
                        name.setPosition(x + w, y + align);
                    } else {
                        name.rotate(+90);
                        name.setPosition(x + w - shift, y + align);
                    }
                    break;
                case 1:
                    if (i == 12 || i == 15 || i == 17) {
                        name.setPosition(x + align, y + h - nh / 2);
                    } else {
                        name.setPosition(x + align, y + h - shift - nh / 2);
                    }
                    break;
                case 2:
                    name.rotate(-90);
                    if (i == 22 || i == 25 || i == 28) {
                        name.setPosition(x, y + h - align);
                    } else {
                        name.setPosition(x + shift, y + h - align);
                    }
                    break;
                case 3:
                    if (i == 33 || i == 35 || i == 36 || i == 38) {
                        name.setPosition(x + align, y);
                    } else {
                        name.setPosition(x + align, y + shift);
                    }
                    break;
            }
        }

        window.draw(name);
    }
    {
        sf::Text curTurn("Cur turn by: ", mainFont);
        curTurn.setPosition(shapes.fieldRects[0].getPosition() + sf::Vector2f(0, shapes.fieldRects[0].getSize().y + 5));
        auto[x, y, w, h] = curTurn.getLocalBounds();
        sf::CircleShape p(h / 2);
        p.setPosition(curTurn.getPosition() + sf::Vector2f(x + w + 20, y));
        p.setFillColor(getColor(curTurnBy));
        window.draw(curTurn);
        window.draw(p);
    }
}

void moveTo(sf::Shape &s, const sf::RectangleShape &to) {
    auto[x, y] = to.getPosition();
    auto[w, h] = to.getSize();
    s.move(x + w / 2, y + h / 2);
}

void SFMLView::drawPlayers(const BoardModel &board) {
    curTurnBy = board.curPlayer;
    for (const auto &player : board.players) {
        int i = player.position;
        int row = i / 10;
        float k = 6;
        bool isSide = row % 2 == 0;
        auto &tile = shapes.fieldRects[i];
        auto[w, h] = isSide ? tile.getSize() : transpose(tile.getSize());
        sf::CircleShape p(w / 20);
        p.setOrigin(p.getRadius(), p.getRadius());
        p.setFillColor(getColor(player.token));
        if (i % 10 == 0) {
            float a = w / k, b = a / 2, c = h / k, d = c / 2;
            switch (player.token) {
                case Token::DOG:
                    p.move(-a + b, c + d);
                    break;
                case Token::HAT:
                    p.move(a + b, -c + d);
                    break;
                case Token::BOOT:
                    p.move(a - b, -c - d);
                    break;
                case Token::CAT:
                    p.move(-a - b, c - d);
                    break;
                case Token::CAR:
                    p.move(a + b, -c - d);
                    break;
                case Token::SHIP:
                    p.move(-a - b, c + d);
                    break;
                case Token::FREE_FIELD:
                    assert(false);
            }
            auto[px, py] = p.getPosition();
            switch (row) {
                case 0:
                    p.setPosition(px, py);
                    break;
                case 1:
                    p.setPosition(-px, py);
                    break;
                case 2:
                    p.setPosition(-px, -py);
                    break;
                case 3:
                    p.setPosition(px, -py);
                    break;
            }
        } else {
            p.setPosition(-w / k, 0);
            switch (player.token) {
                case Token::DOG:
                    p.move(0, h / (2 * k));
                    break;
                case Token::HAT:
                    p.move(0, -h / (2 * k));
                    break;
                case Token::BOOT:
                    p.move(-w / k, -h / (2 * k));
                    break;
                case Token::CAT:
                    p.move(-w / k, h / (2 * k));
                    break;
                case Token::CAR:
                    p.move(-w / (2 * k), -h / k);
                    break;
                case Token::SHIP:
                    p.move(-w / (2 * k), h / k);
                    break;
                case Token::FREE_FIELD:
                    assert(false);
            }
            auto[px, py] = p.getPosition();
            switch (row) {
                case 0:
                    p.setPosition(px, py);
                    break;
                case 1:
                    p.setPosition(py, px);
                    break;
                case 2:
                    p.setPosition(-px, -py);
                    break;
                case 3:
                    p.setPosition(-py, -px);
                    break;
            }
        }
        moveTo(p, tile);
        window.draw(p);
    }
}

void SFMLView::drawMessage() {
    std::lock_guard g(requestMutex);
    switch(messageType) {
        case MessageType::CHANCE:
            {
                auto[W, H] = window.getSize();
                message.setCharacterSize(20);
                auto[nx, ny, nw, nh] = message.getLocalBounds();
                message.setOrigin(nx + nw / 2, ny + nh / 2);
                message.setPosition(sf::Vector2f(float(W) / 2, float(H) / 2));
            }
            break;
        case MessageType::PUBLIC_TREASURY:
            {
                auto[W, H] = window.getSize();
                message.setCharacterSize(20);
                auto[nx, ny, nw, nh] = message.getLocalBounds();
                message.setOrigin(nx + nw / 2, ny + nh / 2);
                message.setPosition(sf::Vector2f(float(W) / 2, float(H) / 2));
            }
            break;
        case MessageType::INFO:
            {
                const auto &baseRect = shapes.fieldRects[0];
                message.setCharacterSize(15);
                message.setPosition(baseRect.getPosition() + /*baseRect.getPoint(1) +*/
                                sf::Vector2f(message.getLocalBounds().width, -message.getLocalBounds().height));
            }
            break;
        case MessageType::DICE:
            {
                auto[W, H] = window.getSize();
                message.setCharacterSize(30);
                auto[nx, ny, nw, nh] = message.getLocalBounds();
                message.setOrigin(nx + nw / 2, ny + nh / 2);
                message.setPosition(sf::Vector2f(float(W) / 2, float(H) / 2));
            }
            break;
    }
    window.draw(message);
}


void SFMLView::monopolyDrawer() {
    if (!isMenu) {
        BoardModel m = getModel();
        drawField(m);
        drawPlayers(m);
        drawMoney(m);
        drawMessage();
        drawCardInfo(m, curCardIndex);
        if (isTradeReq) {
            drawTradeButtons();
        }
        if (isSumReq) {
            enteredT.setString("Enter price: " + enteredText);
            auto[nx, ny, nw, nh] = enteredT.getLocalBounds();
            auto[x, y] = shapes.fieldRects[35].getPosition();

            enteredT.setOrigin(nx + nw / 2, 0);
            enteredT.setPosition(x, y + shapes.fieldRects[35].getSize().y * 2);
            window.draw(enteredT);
        }
        buttons.draw(window);
        window.draw(box);
    }
}

void SFMLView::menuDrawer() {
    drawMenuButtons();
    if (isTokenDraw) {
        drawTokenButtons();
    }
}

void SFMLView::redraw(const Board &board) {
    std::lock_guard g(boardMutex);
    model.update(board);
}

PlayerReply SFMLView::processRequest(Player &p, PlayerRequest req) {
    std::unique_lock g(requestMutex);
    curRequest = std::move(req);
    requestCond.wait(g, [this]() { return bool(this->curReply); });
    assert(curReply);
    return std::move(curReply);
}

void SFMLView::processMessage(Player &p, PlayerMessage mes, MessageType type) {
    std::lock_guard g(requestMutex);
    messageType = type;
    if (type == MessageType::CHANCE) {
        message.setString("CHANCE\n" + mes.message);
    } else if (type == MessageType::PUBLIC_TREASURY) {
        message.setString("PUBLIC TREASURY\n" + mes.message);
    } else
        message.setString(mes.message);

}

NumReply SFMLView::processNum(Player &p) {
    std::unique_lock g(requestMutex);
    for (auto& [act, btn] : actionButtons) {
        btn.deactivate();
    }
    isNumReq = true;
    requestCond.wait(g, [this]() { return bool(this->curNum); });
    assert(curNum);
    return std::move(curNum);
}

SumReply SFMLView::processSum(Player &p) {
    std::unique_lock g(requestMutex);
    for (auto& [act, btn] : actionButtons) {
        btn.deactivate();
    }
    enteredText.clear();
    isSumReq = true;
    requestCond.wait(g, [this]() { return bool(this->curSum); });
    assert(curSum);
    return std::move(curSum);
}

TokenReply SFMLView::processToken(Player &p) {
    std::unique_lock g(requestMutex);
    for (auto& [act, btn] : actionButtons) {
        btn.deactivate();
    }
    isTokenReq = true;
    requestCond.wait(g, [this]() { return bool(this->curToken); });
    assert(curToken);
    return std::move(curToken);
}

PlayerTradeReply SFMLView::processTradeRequest(Player &p, PlayerTradeRequest req) {
    std::unique_lock g(requestMutex);
    for (auto& [act, btn] : actionButtons) {
        btn.deactivate();
    }
    isTradeReq = true;
    requestCond.wait(g, [this]() { return bool(this->curTrade); });
    assert(curTrade);
    if (curTrade->action == PlayerTradeAction::PARTICIPATE) {
        std::cout << "Part\n";
    } else if (curTrade->action == PlayerTradeAction::REFUSE) {
        std::cout << "Refuse\n";
    }
    return std::move(curTrade);
}


void SFMLView::onResize(sf::Event::SizeEvent e) {
    auto[m, M] = e;
    if(m > M) std::swap(m, M);

    window.setView(sf::View(sf::FloatRect(0, 0, e.width, e.height)));
    const size_t side = (40 * m) / 600;
    const size_t longSide = (50 * m) / 600;
    const auto ss = static_cast<float>(side);
    const auto ls = static_cast<float>(longSide);
    const size_t longSideDiff = longSide - side;
    const size_t rowLen = 10;
    const size_t totalSide = side * rowLen + longSideDiff;
    const size_t totalLen = totalSide + longSide;
    const size_t left = (e.width - totalLen) / 2;
    const size_t bot = e.height - (e.height - totalLen) / 2 - longSide;
    const size_t right = left + totalSide;
    const size_t top = bot - totalSide;

    for (size_t i = 0; i < Board::FIELD_SIZE; i++) {
        auto &tile = shapes.fieldRects[i];
        tile.setFillColor(sf::Color::Transparent);
        tile.setOutlineColor(sf::Color::White);
        tile.setOutlineThickness(1);

        const size_t k = i % rowLen;
        if (k == 0) {
            tile.setSize({ls, ls});
            switch (i / rowLen) {
                case 0:
                    tile.setPosition(left, bot);
                    break;
                case 1:
                    tile.setPosition(left, top);
                    break;
                case 2:
                    tile.setPosition(right, top);
                    break;
                case 3:
                    tile.setPosition(right, bot);
                    break;
            }
        } else {
            const auto shift = static_cast<float>(side * k);
            switch (i / rowLen) {
                case 0:
                    tile.setPosition(left, bot - shift);
                    break;
                case 1:
                    tile.setPosition(left + shift + longSideDiff, top);
                    break;
                case 2:
                    tile.setPosition(right, top + shift + longSideDiff);
                    break;
                case 3:
                    tile.setPosition(right - shift, bot);
                    break;
            }
            if ((i / rowLen) % 2 == 0) {
                tile.setSize({ls, ss});
            } else {
                tile.setSize({ss, ls});
            }
        }
        auto [x, y] = shapes.fieldRects[i].getPosition();
        auto [w, h] = shapes.fieldRects[i].getSize();
        fieldButtons.fieldButtons[i].setSize(x, y, w, h);
    }

    const auto& baseRect = shapes.fieldRects[0];
    switch(messageType) {
        case MessageType::INFO:
            message.setPosition(baseRect.getPosition() + baseRect.getPoint(1) + sf::Vector2f(5, -message.getLocalBounds().height * 2));
            break;
        case MessageType::DICE:
            {
                auto[W, H] = window.getSize();
                message.setPosition(sf::Vector2f(float(W) / 2, float(H) / 2));
            }
            break;
        case MessageType::CHANCE:
            {
                auto[W, H] = window.getSize();
                message.setPosition(sf::Vector2f(float(W) / 2, float(H) / 2));
            }
            break;
        case MessageType::PUBLIC_TREASURY:
            {
                auto[W, H] = window.getSize();
                message.setPosition(sf::Vector2f(float(W) / 2, float(H) / 2));
            }
            break;
    }
}

void SFMLView::drawMoney(const BoardModel &board) {
    auto [W, H] = window.getSize();
    sf::Text money("Money:", mainFont);
    std::vector<std::pair<sf::Text, Token>> moneyTexts;
    float maxW = money.getLocalBounds().width, maxH = money.getLocalBounds().height;
    for(const auto& player : board.players) {
        moneyTexts.emplace_back(sf::Text(" - " + std::to_string(player.money), mainFont), player.token);
        maxW = std::max(maxW, moneyTexts.back().first.getLocalBounds().width);
        maxH = std::max(maxH, moneyTexts.back().first.getLocalBounds().height);
    }
    maxH *= 2;
    const int shift = 30;
    money.setPosition(W - maxW - maxH / 2 - shift, 10);
    for(size_t i = 0; i < moneyTexts.size(); i++) {
        auto& [mt, tok] = moneyTexts[i];
        mt.setPosition(W - maxW - shift, 10 + static_cast<float>(i + 1) * maxH);
        sf::CircleShape p(maxH / 4);
        float x = W - maxW - maxH / 2 - shift;
        float y = 10 + static_cast<float>(i + 1) * maxH /*+ maxH / 4 */;
        p.setPosition(x, y + mt.getLocalBounds().top);
        p.setFillColor(getColor(tok));
        tokenButtons[tok].setScale(sf::Vector2f(0.3, 0.3));
        auto[bx, by] = tokenButtons[tok].getSize();
        tokenButtons[tok].setPosition(sf::Vector2f(x - bx - shift / 2, y));
        tokenButtons[tok].drawTo(window);
        tokenButtons[tok].activate();
        window.draw(mt);
        window.draw(p);
    }
    window.draw(money);
}

void SFMLView::drawToken(const BoardModel &board) {

}

void SFMLView::drawCardInfo(const BoardModel &board, std::optional<std::size_t> index) {
    auto baseRect = shapes.fieldRects[0];
    auto[w, h] = baseRect.getSize();
    w *= 3, h *= 4;
    baseRect.setSize(sf::Vector2f(w, h));
    auto[wx, wy] = window.getSize();
    baseRect.setPosition(sf::Vector2f(float(wx), float(wy)) - sf::Vector2f(w + 1, h + 1));
    if (index.has_value()) {
        const auto &fieldTile = board.field[index.value()];
        //color
        sf::Color color;
        if (fieldTile.color.has_value()) {
            color = getColor(fieldTile.color.value());
        } else {
            color.r = 128;
            color.g = 128;
            color.b = 128;
        }
        const float size = h / 8;
        sf::RectangleShape colorRect(sf::Vector2f(w, size));
        colorRect.setFillColor(color);
        colorRect.setPosition(sf::Vector2f(float(wx), float(wy)) - sf::Vector2f(w + 1, h + 1));
        window.draw(colorRect);


        {   // name

            std::string s = std::string(fieldTile.name);
            sf::Text title(s, mainFont);
            int fsize = 18;
            title.setCharacterSize(fsize);
          //  while (!doesFit(title, w)) {
            //    title.setCharacterSize(--fsize);
          //  }
            auto[nx, ny, nw, nh] = title.getLocalBounds();
            auto[x, y] = baseRect.getPosition();

            title.setOrigin(nx + nw / 2, 0);
            title.setPosition(x + w / 2, y);
            window.draw(title);
        }
        //
        std::string cardText;
        if (fieldTile.cost.has_value()) {
            cardText += "Cost: ";
            cardText += std::to_string(fieldTile.cost.value());
            cardText += "\n";
        }

        if (fieldTile.costOfParking.has_value()) {
            cardText += "Cost of Parking: ";
            cardText += std::to_string(fieldTile.costOfParking.value());
            cardText += "\n";
        }

        if (fieldTile.owner.has_value()) {
            cardText += "Owner: ";
            sf::Color c = getColor(fieldTile.owner.value());
            if (c == sf::Color::White) {
                cardText += "White";
            } else if (c == sf::Color::Red) {
                cardText += "Red";
            } else if (c == sf::Color::Blue) {
                cardText += "Blue";
            } else if (c == sf::Color::Cyan) {
                cardText += "Cyan";
            } else if (c == sf::Color::Magenta) {
                cardText += "Magenta";
            } else if (c == sf::Color::Green) {
                cardText += "Green";
            }
            cardText += "\n";
        }

        cardText += "Mortgage: ";
        if (fieldTile.isMortgaged)
            cardText += "Yes\n";
        else
            cardText += "No\n";

        if (fieldTile.numberOfHouses.has_value()) {
            cardText += "Num of houses: ";
            cardText += std::to_string(fieldTile.numberOfHouses.value());
            cardText += "\n";
        }

        if (fieldTile.costPerHouse.has_value()) {
            cardText += "Cost per house: ";
            cardText += std::to_string(fieldTile.costPerHouse.value());
            cardText += "\n";
        }

        sf::Text text(cardText, mainFont);
        int fsize = 16;
        text.setCharacterSize(fsize);
        auto[x, y] = baseRect.getPosition();
        y = wy - h + size;
      //  text.setOrigin(nx + nw / 2, 0);
        text.setPosition(x, y);
        window.draw(text);

    }

    window.draw(baseRect);
}

void SFMLView::handleRequest() {
    PlayerRequest req;
    {
        std::lock_guard g(requestMutex);
        if(!curRequest.has_value()) return;
        req = std::move(curRequest.value());
        curRequest.reset();
    }
    for (auto& [act, btn] : actionButtons) {
        btn.deactivate();
    }
    for(auto action : req.availableActions) {
        std::cout << "ACTIVE: ";
        switch(action) {
            case PlayerAction::PAY_TAX:
                std::cout << "PAY_TAX"; break;
            case PlayerAction::BUY_FIELD:
                std::cout << "BUY_FIELD"; break;
            case PlayerAction::SELL_FIELD:
                std::cout << "SELL_FIELD"; break;
            case PlayerAction::BUY_HOUSE:
                std::cout << "BUY_HOUSE"; break;
            case PlayerAction::SELL_HOUSE:
                std::cout << "SELL_HOUSE"; break;
            case PlayerAction::BUY_HOTEL:
                std::cout << "BUY_HOTEL"; break;
            case PlayerAction::SELL_HOTEL:
                std::cout << "SELL_HOTEL"; break;
            case PlayerAction::BUY_BACK_PROPERTY:
                std::cout << "BUY_BACK_PROPERTY"; break;
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
            case PlayerAction::FINISH_GAME:
                std::cout << "FINISH_GAME"; break;
        }
        std::cout << std::endl;
        if(actionButtons.count(action)) {
            actionButtons.at(action).activate();
        }
    }
    std::cout << "_____________________________\n";
}

BoardModel SFMLView::getModel() {
    std::lock_guard g(boardMutex);
    return model; //copy
}

void SFMLView::makeReply(PlayerReply rep) {
    std::lock_guard g(requestMutex);
    curReply = std::move(rep);
    requestCond.notify_all();
}

void SFMLView::makeNumReply(NumReply rep) {
    std::lock_guard g(requestMutex);
    curNum = std::move(rep);
    requestCond.notify_all();
}

void SFMLView::makeSumReply(SumReply rep) {
    std::lock_guard g(requestMutex);
    curSum = std::move(rep);
    requestCond.notify_all();
}

void SFMLView::makeTokenReply(TokenReply rep) {
    std::lock_guard g(requestMutex);
    curToken = std::move(rep);
    requestCond.notify_all();
}

void SFMLView::makeTradeReply(PlayerTradeReply rep) {
    std::lock_guard g(requestMutex);
    curTrade = std::move(rep);
    requestCond.notify_all();
}

void SFMLView::addActionButton(PlayerAction action,
                               const std::string& texture,
                               std::string tooltip,
                               const std::function<void()>& handler) {
    auto btn = std::make_unique<SpriteButton>();
    size_t i = actionButtons.size();
    btn->setPosition(static_cast<float>(10 + 80 * (i % 2)), static_cast<float>(10 + 80 * (i / 2)));
   // btn->setScale({0.8f, 0.8f});
    btn->setTexture(texture);
    actionButtons.emplace(action, *btn);
    auto[x, y] = btn->getPosition();
    auto[w, h] = btn->getSize();
    tooltips.addTooltip({std::move(tooltip), x, y, w, h});
    buttons.addButton(std::move(btn), [handler](auto e) { handler(); return true; });
}

void SFMLView::drawMenuButtons() {
    auto[w, h] = window.getSize();
    float shift = float(h) / NUM_OF_BUTTONS;
    sf::Vector2f buttonSize = sf::Vector2f(float(w) / 3, float(h) / 10);
    menuButtons[0] = ButtonText("START GAME", buttonSize, 18, sf::Color(130, 130, 130), sf::Color::White);
    menuButtons[1] = ButtonText("ADD PLAYER", buttonSize, 18, sf::Color(130, 130, 130), sf::Color::White);
    menuButtons[2] = ButtonText("ABOUT", buttonSize, 18, sf::Color(130, 130, 130), sf::Color::White);
    menuButtons[3] = ButtonText("EXIT", buttonSize, 18, sf::Color(130, 130, 130), sf::Color::White);

    for (auto & menuButton : menuButtons)
        menuButton.setFont(mainFont);
    float start = shift / 2;

    menuButtons[0].setPosition(sf::Vector2f(w / 2, start));
    menuButtons[1].setPosition(sf::Vector2f(w / 2, start + shift));
    menuButtons[2].setPosition(sf::Vector2f(w / 2, start + shift * 2));
    menuButtons[3].setPosition(sf::Vector2f(w / 2, start + shift * 3));

    for (auto & menuButton : menuButtons)
        menuButton.drawTo(window);
}

void SFMLView::drawTokenButtons() {
    auto[w, h] = window.getSize();
    float shift = h / 3;
    float start = shift / 4;
    float x1 = w / 6;
    float x2 = w * 5 / 6;
    tokenButtons[Token::DOG].setPosition(sf::Vector2f(x1, start));
    tokenButtons[Token::DOG].setScale(sf::Vector2f(0.7,0.7));
    tokenButtons[Token::BOOT].setPosition(sf::Vector2f(x1, start + shift));
    tokenButtons[Token::BOOT].setScale(sf::Vector2f(0.7,0.7));
    tokenButtons[Token::CAR].setPosition(sf::Vector2f(x1, start + shift * 2));
    tokenButtons[Token::CAR].setScale(sf::Vector2f(0.7,0.7));

    tokenButtons[Token::CAT].setPosition(sf::Vector2f(x2, start));
    tokenButtons[Token::CAT].setScale(sf::Vector2f(0.7,0.7));
    tokenButtons[Token::HAT].setPosition(sf::Vector2f(x2, start + shift));
    tokenButtons[Token::HAT].setScale(sf::Vector2f(0.7,0.7));
    tokenButtons[Token::SHIP].setPosition(sf::Vector2f(x2, start + shift * 2));
    tokenButtons[Token::SHIP].setScale(sf::Vector2f(0.7,0.7));

    for (auto &t : tokenButtons) {
        t.second.drawTo(window);
    }
}

void SFMLView::drawTradeButtons() {
    auto[w, h] = window.getSize();
    sf::Vector2f buttonSize = sf::Vector2f(float(w) / 10, float(h) / 20);
    participateInTrade[0] = ButtonText("PARTICIPATE", buttonSize, 15, sf::Color::Green, sf::Color::White);
    participateInTrade[1] = ButtonText("REFUSE", buttonSize, 15, sf::Color::Red, sf::Color::White);

    for (auto &button : participateInTrade)
        button.setFont(mainFont);

    float y = shapes.fieldRects[0].getPosition().y + shapes.fieldRects[0].getSize().y * 2;
    participateInTrade[0].setPosition(sf::Vector2f(shapes.fieldRects[0].getPosition().x + buttonSize.x / 2, y));
    participateInTrade[1].setPosition(sf::Vector2f(shapes.fieldRects[30].getPosition().x + shapes.fieldRects[30].getSize().x - buttonSize.x / 2, y));

    for (auto &button : participateInTrade)
        button.drawTo(window);
}







