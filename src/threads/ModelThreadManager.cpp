#include "threads/ModelThreadManager.h"

namespace Monopoly::Threads {


    void ModelThreadManager::sync(const Board& board) {
        std::lock_guard g(boardMutex);
        model.update(board);
        dirty = true;
    }

    void ModelThreadManager::sync(BoardModel board) {
        std::lock_guard g(boardMutex);
        model = std::move(board);
        dirty = true;
    }

    PlayerReply ModelThreadManager::processRequest(PlayerRequest req) {
        std::unique_lock g(requestMutex);
        curRequest = std::move(req);
        requestCond.wait(g, [this]() { return bool(this->curReply); });
        assert(curReply);
        return std::move(curReply);
    }

    void ModelThreadManager::processRequestAsync(PlayerRequest req) {
        std::unique_lock g(requestMutex);
        curRequest = std::move(req);
    }

    void ModelThreadManager::processMessage(PlayerMessage mes) {
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

    std::optional<PlayerReply> ModelThreadManager::getReply() {
        std::lock_guard g(requestMutex);
        if(curReply) return {std::move(curReply)};
        else return {};
    }

    void ModelThreadManager::sendReply(PlayerReply reply) {
        std::lock_guard g(requestMutex);
        curReply = std::move(reply);
        requestCond.notify_all();
    }

    BoardModel ModelThreadManager::getBoard() {
        std::lock_guard g(boardMutex);
        dirty = false;
        return model; //copy
    }

    bool ModelThreadManager::isDirty() {
        std::lock_guard g(boardMutex);
        return dirty;
    }

    ModelThreadManager::ModelThreadManager(ModelThreadManager&& other) noexcept
    : model(std::move(other.model)), dirty(other.dirty), curRequest(std::move(other.curRequest)),
      curMessage(std::move(other.curMessage)), curReply(std::move(other.curReply)) {}


}