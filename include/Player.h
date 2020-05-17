#ifndef PLAYER_H
#define PLAYER_H

#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include "View.h"
#include <string>
#include <vector>
#include <variant>
#include <memory>

class Player {
public:
    explicit Player(Token token, std::string name);
    virtual ~Player() = default;
    virtual PlayerReply sendRequest(PlayerRequest request) = 0;
    virtual PlayerTradeReply sendTradeRequest(PlayerTradeRequest request) = 0;
    virtual void sendMessage(PlayerMessage request, MessageType type) = 0;
    virtual NumReply sendNum() = 0;
    virtual SumReply sendSum() = 0;
    virtual void sync(const Board& board) = 0;

    const Token token;
    const std::string name;
};

class NetworkPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sendMessage(PlayerMessage request, MessageType type) override;
    NumReply sendNum() override;
    SumReply sendSum() override;
    PlayerTradeReply sendTradeRequest(PlayerTradeRequest request) override;
    void sync(const Board& board) override;
};

class BotPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sendMessage(PlayerMessage request, MessageType type) override;
    NumReply sendNum() override;
    SumReply sendSum() override;
    PlayerTradeReply sendTradeRequest(PlayerTradeRequest request) override;
    void sync(const Board& board) override;
};

class LocalPlayer final : public Player {
public:
    explicit LocalPlayer(Token token, std::string name, SFMLView& view);
    PlayerReply sendRequest(PlayerRequest request) override;
    void sendMessage(PlayerMessage request, MessageType type) override;
    NumReply sendNum() override;
    SumReply sendSum() override;
    PlayerTradeReply sendTradeRequest(PlayerTradeRequest request) override;
    void sync(const Board& board) override;

private:
    SFMLView& view;
    SFMLView& gameView();
};

#endif //PLAYER_H
