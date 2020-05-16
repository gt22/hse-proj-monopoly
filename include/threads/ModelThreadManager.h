#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include "BoardModel.h"

namespace Monopoly::Threads {
    class ModelThreadManager {
    public:
        void sync(const Board& board);

        bool isDirty();
        void clearDirty();

        PlayerReply processRequest(Player& p, PlayerRequest req);

        void processMessage(Player& p, PlayerMessage mes);

        std::optional<PlayerRequest> getRequest();

        std::optional<PlayerMessage> getMessage();

        void sendReply(PlayerReply reply);

        BoardModel getBoard();

    private:
        std::mutex boardMutex, requestMutex;
        std::condition_variable requestCond;

        BoardModel model;
        bool dirty;

        std::optional<PlayerRequest> curRequest;
        std::optional<PlayerMessage> curMessage;
        PlayerReply curReply;
    };
}