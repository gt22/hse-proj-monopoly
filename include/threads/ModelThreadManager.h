#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include "BoardModel.h"

namespace Monopoly::Threads {
    class ModelThreadManager {
    public:

        ModelThreadManager() = default;
        ModelThreadManager(ModelThreadManager&& other) noexcept;

        void sync(const Board& board);
        void sync(BoardModel board);

        bool isDirty(); // set by sync, reset by getBoard

        PlayerReply processRequest(PlayerRequest req);

        void processRequestAsync(PlayerRequest req);

        void processMessage(PlayerMessage mes);

        std::optional<PlayerRequest> getRequest();

        std::optional<PlayerMessage> getMessage();

        std::optional<PlayerReply> getReply();

        void sendReply(PlayerReply reply);

        BoardModel getBoard();

    private:
        std::mutex boardMutex, requestMutex;
        std::condition_variable requestCond;

        BoardModel model;
        bool dirty = false;

        std::optional<PlayerRequest> curRequest;
        std::optional<PlayerMessage> curMessage;
        PlayerReply curReply;
    };
}