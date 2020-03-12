#ifndef PLAYER_H
#define PLAYER_H

#include "MonopolyFwd.h"
#include "PlayerRequests.h"
#include <string>
#include <vector>
#include <variant>

enum class Token : std::size_t {
    DOG,
    TYPE2,
    TYPE3
};

class Player {
public:
    explicit Player(Token token);
    virtual ~Player() = default;
    virtual PlayerReply sendRequest(PlayerRequest request) = 0;
    virtual void sync(const Board& board) = 0;

    const Token token;
};

class NetworkPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const Board& board) override;
};

class BotPlayer final : public Player {
public:
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const Board& board) override;
};

class LocalPlayer final : public Player {
public:
    explicit LocalPlayer(Token token, std::shared_ptr<ViewHolder> view);
    PlayerReply sendRequest(PlayerRequest request) override;
    void sync(const Board& board) override;

private:
    std::shared_ptr<ViewHolder> view;
    MonopolyView& gameView();
};

#endif //PLAYER_H
