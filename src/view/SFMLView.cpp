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
    }
}

SFMLView::SFMLView(Manager &manager) : manager(manager) {
    window.create(sf::VideoMode(800, 600), "Monopoly");
    window.setFramerateLimit(60);
    mainFont.loadFromFile("Ubuntu-R.ttf");
    // text button
    ButtonText btn2("1", {30, 30}, 30, sf::Color::Green, sf::Color::Black);
    btn1 = std::move(btn2);
    btn1.setFont(mainFont);
    btn1.setPosition({10, 30});
    //

    // sprite button

    btnEndTurn.setTexture("images/END_TURN.png");
    btnEndTurn.setPosition({10, 10});
    btnEndTurn.setScale({0.7, 0.7});

    btnExitGame.setTexture("images/EXIT_GAME.png");
    btnExitGame.setPosition({70, 10});
    btnExitGame.setScale({0.7, 0.7});

    btnPayTax.setTexture("images/PAY_TAX.png");
    btnPayTax.setPosition({10, 70});
    btnPayTax.setScale({0.7, 0.7});

    btnRollDice.setTexture("images/ROLL_DICE.png");
    btnRollDice.setPosition({70, 70});
    btnRollDice.setScale({0.7, 0.7});

    btnTakeCard.setTexture("images/TAKE_CARD.png");
    btnTakeCard.setPosition({10, 140});
    btnTakeCard.setScale({0.7, 0.7});

    btnUseCard.setTexture("images/USE_CARD.png");
    btnUseCard.setPosition({70, 140});
    btnUseCard.setScale({0.7, 0.7});


    events.addHandler<sf::Event::Closed>([this]() { window.close(); });
    events.addHandler<sf::Event::Resized>([this](auto e) { onResize(e); });
    events.addHandler<sf::Event::MouseButtonPressed>([this](sf::Event::MouseButtonEvent e) {
        if (e.button == sf::Mouse::Left) {
            if (this->btnEndTurn.isMouseOver(window)) {
                if (this->btnEndTurn.getClickability()) {
                    this->manager.getBoard().getPlayer(curTurnBy).newPosition(rand() % 6 + 1);
                    switch (curTurnBy) {

                        case Token::DOG:
                            curTurnBy = Token::HAT;
                            break;
                        case Token::HAT:
                            curTurnBy = Token::BOOT;
                            break;
                        case Token::BOOT:
                            curTurnBy = Token::CAT;
                            break;
                        case Token::CAT:
                            curTurnBy = Token::CAR;
                            break;
                        case Token::CAR:
                            curTurnBy = Token::SHIP;
                            break;
                        case Token::SHIP:
                            curTurnBy = Token::DOG;
                            break;
                        case Token::FREE_FIELD:
                            assert(false);
                    }
                }
            } else if (this->btnExitGame.isMouseOver(window)) {
                if (this->btnExitGame.getClickability())
                    std::cout << "EXIT GAME" << "\n";
            } else if (this->btnPayTax.isMouseOver(window)) {
                if (this->btnPayTax.getClickability())
                    std::cout << "PAY TAX" << "\n";
            } else if (this->btnRollDice.isMouseOver(window)) {
                if (this->btnRollDice.getClickability())
                    std::cout << "ROLL DICE" << "\n";
            } else if (this->btnTakeCard.isMouseOver(window)) {
                if (this->btnTakeCard.getClickability())
                    std::cout << "TAKE CARD" << "\n";
            } else if (this->btnUseCard.isMouseOver(window)) {
                if (this->btnUseCard.getClickability())
                    std::cout << "USE CARD" << "\n";
            }
        } else if (e.button == sf::Mouse::Right) {
            if (this->btnEndTurn.isMouseOver(window)) {
                this->btnEndTurn.toggleClickability();
            } else if (this->btnExitGame.isMouseOver(window)) {
                this->btnExitGame.toggleClickability();
            } else if (this->btnPayTax.isMouseOver(window)) {
                this->btnPayTax.toggleClickability();
            } else if (this->btnRollDice.isMouseOver(window)) {
                this->btnRollDice.toggleClickability();
            } else if (this->btnTakeCard.isMouseOver(window)) {
                this->btnTakeCard.toggleClickability();
            } else if (this->btnUseCard.isMouseOver(window)) {
                this->btnUseCard.toggleClickability();
            }
        }
        redraw(this->manager.getBoard());
    });

    //  events.addHandler<sf::Event::MouseButtonPressed>([this](sf::Event::MouseButtonEvent e){ tmp++; });
    /*  events.addHandler<sf::Event::MouseButtonPressed>([this](sf::Event::MouseButtonEvent e){
          if (this->btn1.isMouseOver(window)) {
              std::cout << "Pressed " << "\n";
          }
      });*/
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

SFMLView::~SFMLView() {
    windowMutex.lock();
    shouldClose = true;
    windowMutex.unlock();
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
        sf::Event e{};
        while (window.pollEvent(e)) {
            events.handleEvent(e);
        }
        window.clear();
        draw();
        btnEndTurn.drawTo(window);
        btnExitGame.drawTo(window);
        btnPayTax.drawTo(window);
        btnRollDice.drawTo(window);
        btnTakeCard.drawTo(window);
        btnUseCard.drawTo(window);
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

        if(fieldTile.color.has_value()) {
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

void SFMLView::draw() {
    BoardModel m = getModel();
    drawField(m);
    drawPlayers(m);
    drawMoney(m);
}

void SFMLView::redraw(const Board &board) {
    std::lock_guard g(boardMutex);
    model.update(board);
}

PlayerReply SFMLView::processRequest(Player &p, PlayerRequest req) {

    btnUseCard.makeUnclickable();
    btnTakeCard.makeUnclickable();
    btnRollDice.makeUnclickable();
    btnPayTax.makeClickable();
    btnExitGame.makeClickable();
    btnEndTurn.makeClickable();

    /* for (std::size_t i = 0; i < req.availableActions.size(); i++) {
         if (req.availableActions[i] == PlayerAction::PAY_TAX) {
             btnPayTax.makeUnclickable();
         } else if (req.availableActions[i] == PlayerAction::USE_CARD) {
             btnUseCard.makeUnclickable();
         } else if (req.availableActions[i] == PlayerAction::TAKE_CARD) {
             btnTakeCard.makeUnclickable();
         } else if (req.availableActions[i] == PlayerAction::ROLL_DICE) {
             btnRollDice.makeUnclickable();
         } else if (req.availableActions[i] == PlayerAction::EXIT_GAME) {
             btnExitGame.makeUnclickable();
         } else if (req.availableActions[i] == PlayerAction::END_TURN) {
             btnEndTurn.makeUnclickable();
         }
     }
 */
    //
    return std::make_unique<EndTurnReply>();
}

void SFMLView::processMessage(Player &p, PlayerMessage mes) {

}

void SFMLView::onResize(sf::Event::SizeEvent e) {
    window.setView(sf::View(sf::FloatRect(0, 0, e.width, e.height)));
    const size_t side = (40 * e.width) / 800;
    const size_t longSide = (50 * e.width) / 800;
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

BoardModel SFMLView::getModel() {
    boardMutex.lock();
    BoardModel m = model;
    boardMutex.unlock();
    return m;
}
