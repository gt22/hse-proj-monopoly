#include "threads/ModelThreadManager.h"

namespace Monopoly::Threads {


    void ModelThreadManager::sync(const Board& board) {
        std::lock_guard g(boardMutex);
        model.update(board);
        dirty = true;
    }

    PlayerReply ModelThreadManager::processRequest(Player& p, PlayerRequest req) {
        std::unique_lock g(requestMutex);
        curRequest = std::move(req);
        requestCond.wait(g, [this]() { return bool(this->curReply); });
        assert(curReply);
        return std::move(curReply);
    }

    void ModelThreadManager::processMessage(Player& p, PlayerMessage mes) {
        std::lock_guard g(requestMutex);
        curMessage = std::move(mes);
    }

    std::optional<PlayerRequest> ModelThreadManager::getRequest() {
        std::lock_guard g(requestMutex);
        if(!curRequest.has_value()) return {};
        std::optional<PlayerRequest> ret(std::move(curRequest.value()));
        curRequest.reset();
        return ret;
    }

    std::optional<PlayerMessage> ModelThreadManager::getMessage() {
        std::lock_guard g(requestMutex);
        if(!curMessage.has_value()) return {};
        std::optional<PlayerMessage> ret(std::move(curMessage.value()));
        curMessage.reset();
        return ret;
    }

    void ModelThreadManager::sendReply(PlayerReply reply) {
        std::lock_guard g(requestMutex);
        curReply = std::move(reply);
        requestCond.notify_all();
    }

    BoardModel ModelThreadManager::getBoard() {
        std::lock_guard g(boardMutex);
        return model; //copy
    }

    bool ModelThreadManager::isDirty() {
        std::lock_guard g(boardMutex);
        return dirty;
    }

    void ModelThreadManager::clearDirty() {
        std::lock_guard g(boardMutex);
        dirty = false;
    }


}