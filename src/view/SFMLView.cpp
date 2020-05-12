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
            return sf::Color::White;
        case Token::HAT:
            return sf::Color::Red;
        case Token::BOOT:
            return sf::Color::Blue;
        case Token::CAT:
            return sf::Color::Cyan;
        case Token::CAR:
            return sf::Color::Magenta;
        case Token::SHIP:
            return sf::Color::Green;
        case Token::FREE_FIELD:
            return sf::Color::Transparent;
    }
    return sf::Color::Transparent;
}

sf::Color getColor(Color color) {
    switch(color) {

        case Color::COL1:
            return sf::Color(0x6e0000FF);
        case Color::COL2:
            return sf::Color(0x2a7ce8FF);
        case Color::COL3:
            return sf::Color(0xff3b6fFF);
        case Color::COL4:
            return sf::Color(0xff9d3bFF);
        case Color::COL5:
            return sf::Color(0xe82a2aFF);
        case Color::COL6:
            return sf::Color(0xeded2bFF);
        case Color::COL7:
            return sf::Color(0x0213c9FF);
        case Color::COL8:
            return sf::Color(0x4f088cFF);
        case Color::NO_COL:
            return sf::Color::Transparent;
    }
    return sf::Color::Transparent;
}

SFMLView::SFMLView(Manager &manager) : manager(manager) {
    window.create(sf::VideoMode(800, 600), "Monopoly");
    window.setFramerateLimit(60);
    mainFont.loadFromFile("Ubuntu-R.ttf");

    box.setFont(mainFont);
    box.setCharacterSize(15);
    box.setFillColor(sf::Color::White);
    message.setFont(mainFont);
    message.setCharacterSize(20);
    message.setFillColor(sf::Color::White);

    // sprite button

    addActionButton(PlayerAction::END_TURN,
                    "images/END_TURN.png",
                    "By pressing this button you will finish your turn",
                    makeReplyGenerator<EndTurnReply>());
    addActionButton(PlayerAction::EXIT_GAME,
                    "images/EXIT_GAME.png",
                    "By pressing this button you will exit the game",
                    makeReplyGenerator<ExitGameReply>());
    addActionButton(PlayerAction::PAY_TAX,
                    "images/PAY_TAX.png",
                    "By pressing this button you will pay tax",
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
    addActionButton(PlayerAction::ROLL_DICE,
                    "images/ROLL_DICE.png",
                    "By pressing this button you will roll dice",
                    makeReplyGenerator<RollDiceReply>());
    addActionButton(PlayerAction::TAKE_CARD,
                    "images/TAKE_CARD.png",
                    "By pressing this button you will take card",
                    makeReplyGenerator<TakeCardReply>());
    addActionButton(PlayerAction::USE_CARD,
                    "images/USE_CARD.png",
                    "By pressing this button you will use card",
                    makeReplyGenerator<UseCardReply>());
    addActionButton(PlayerAction::START_TRADE,
                    "images/START_TRADE.png",
                    "By pressing this button you will start trade",
                    makeReplyGenerator<StartTradeReply>());
    addActionButton(PlayerAction::MORTGAGE_HOLDINGS,
                    "images/MORTGAGE_HOLDINGS.png",
                    "By pressing this button you will get mortgage",
                    makeReplyGenerator<MortgageHoldingsReply>());


    events.addHandler<sf::Event::Closed>([this]() { window.close(); });
    events.addHandler<sf::Event::Resized>([this](auto e) { onResize(e); });
    events.addHandler<sf::Event::MouseMoved>([this](sf::Event::MouseMoveEvent e) {
        box.setString(std::string(this->tooltips.getTooltip(e)));
        box.setPosition(sf::Vector2f(20, window.getSize().y - box.getLocalBounds().height * 2));
    });
    events.addHandler<sf::Event::MouseButtonPressed>([this](sf::Event::MouseButtonEvent e) {
        buttons.handle(e);
    });

    onResize({window.getSize().x, window.getSize().y});

    //TODO: temp
    curTurnBy = Token::DOG;
    manager.addPlayer(std::make_unique<LocalPlayer>(Token::DOG, "Player 1", *this));
    manager.addPlayer(std::make_unique<LocalPlayer>(Token::HAT, "Player 2", *this));
    manager.addPlayer(std::make_unique<LocalPlayer>(Token::BOOT, "Player 3", *this));
    manager.addPlayer(std::make_unique<LocalPlayer>(Token::CAR, "Player 4", *this));
    manager.addPlayer(std::make_unique<LocalPlayer>(Token::CAT, "Player 5", *this));
    manager.addPlayer(std::make_unique<LocalPlayer>(Token::SHIP, "Player 6", *this));
    manager.startGame();
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
        draw();
        buttons.draw(window);
        window.draw(box);
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
                case 3: colorRect.setPosition(viewTile.getPosition() + sf::Vector2f(h, 0));
            }
            window.draw(colorRect);
        }

        if (fieldTile.owner.has_value()) {
            sf::Color color = getColor(fieldTile.owner.value());
            sf::RectangleShape ownerLabel(sf::Vector2f(w / 6, w / 6));
//        //    p.setOrigin(p.getRadius(), p.getRadius());
            ownerLabel.setFillColor(color);
            ownerLabel.setPosition(viewTile.getPosition() /*+ sf::Vector2f(w / 9, w / 9) */);
            window.draw(ownerLabel);
        }

        std::string s = std::string(fieldTile.name);
        sf::Text name(s, mainFont);
        int fsize = 11;
        name.setCharacterSize(fsize);
        while (!doesFit(name, h * 0.8f)) {
            auto lastSpace = s.rfind(' ');
            if (lastSpace == std::string::npos) break;
            s.replace(lastSpace, 1, "\n");
            name.setString(s);
        }
        while (doesFit(name, h * 0.7f)) {
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
                    name.rotate(+90);
                    name.setPosition(x + w - shift, y + align);
                    break;
                case 1:
                    name.setPosition(x + align, y + h - shift - nh / 2);
                    break;
                case 2:
                    name.rotate(-90);
                    name.setPosition(x + shift, y + h - align);
                    break;
                case 3:
                    name.setPosition(x + align, y + shift);
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
            break;
        case MessageType::PUBLIC_TREASURY:
            break;
        case MessageType::INFO:
            {
                const auto &baseRect = shapes.fieldRects[0];
                message.setCharacterSize(15);
                message.setPosition(baseRect.getPosition() + baseRect.getPoint(1) +
                                sf::Vector2f(5, -message.getLocalBounds().height * 2));
            }
            break;
        case MessageType::DICE:
            {
                auto[W, H] = window.getSize();
                message.setCharacterSize(30);
                message.setPosition(sf::Vector2f(float(W), float(H)) + sf::Vector2f(5, -message.getLocalBounds().height * 2));
            }
            break;
    }
    window.draw(message);
}


void SFMLView::draw() {
    BoardModel m = getModel();
    drawField(m);
    drawPlayers(m);
    drawMoney(m);
    drawMessage();
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
    message.setString(mes.message);
}

NumReply SFMLView::processNum(Player &p) {
    std::cout << "\n 1 line of SFMLView::processNum \n";
    std::unique_lock g(requestMutex);
    std::cout << "\n 2 line of SFMLView::processNum \n";
    makeNumReply(std::make_unique<NumReplyData>(2));
    std::cout << "\n 3 line of SFMLView::processNum \n";
    requestCond.wait(g, [this]() { return bool(this->curNum); });
    std::cout << "\n 4 line of SFMLView::processNum \n";
    assert(curNum);
    std::cout << "\n 5 line of SFMLView::processNum \n";
    return std::move(curNum);
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

    }

    const auto& baseRect = shapes.fieldRects[0];
    message.setPosition(baseRect.getPosition() + baseRect.getPoint(1) + sf::Vector2f(5, -message.getLocalBounds().height * 2));
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
    const int shift = 30;
    money.setPosition(W - maxW - maxH / 2 - shift, 10);
    for(size_t i = 0; i < moneyTexts.size(); i++) {
        auto& [mt, tok] = moneyTexts[i];
        mt.setPosition(W - maxW - shift, 10 + static_cast<float>(i + 1) * maxH);
        sf::CircleShape p(maxH / 4);
        p.setPosition(W - maxW - maxH / 2 - shift, 10 + static_cast<float>(i + 1) * maxH + mt.getLocalBounds().top + maxH / 4);
        p.setFillColor(getColor(tok));
        window.draw(mt);
        window.draw(p);
    }
    window.draw(money);
}

void SFMLView::handleRequest() {
    PlayerRequest req;
    {
        std::lock_guard g(requestMutex);
        if(!curRequest.has_value()) return;
        req = std::move(curRequest.value());
        curRequest.reset();
    }
   // curTurnBy =
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
  //  std::lock_guard g(requestMutex);
    curNum = std::move(rep);
    numCond.notify_all();
}

void SFMLView::addActionButton(PlayerAction action,
                               const std::string& texture,
                               std::string tooltip,
                               const std::function<void()>& handler) {
    auto btn = std::make_unique<SpriteButton>();
    size_t i = actionButtons.size();
    btn->setPosition(static_cast<float>(10 + 60 * (i % 2)), static_cast<float>(10 + 60 * (i / 2)));
    btn->setScale({0.7f, 0.7f});
    btn->setTexture(texture);
    actionButtons.emplace(action, *btn);
    auto[x, y] = btn->getPosition();
    auto[w, h] = btn->getSize();
    tooltips.addTooltip({std::move(tooltip), x, y, w, h});
    buttons.addButton(std::move(btn), [handler](auto e) { handler(); return true; });
}






