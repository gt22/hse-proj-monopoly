#include "network/MonopolyServer.h"

#include <utility>
#include "network/internal/MessageFormat.h"
#include "network/MonopolySerialization.h"

namespace Monopoly::Network {

    ClientHandler::ClientHandler(ClientId id, const sockpp::inet_address& addr, sockpp::tcp_socket socket,
                                 std::function<void(ClientId)> onDeath)
            : id(id), addr(addr), socket(std::move(socket)), onDeath(std::move(onDeath)) {}

    ClientHandler::ClientHandler(ClientHandler&& other) noexcept
            : id(other.id), th(std::move(other.th)), addr(other.addr), socket(std::move(other.socket)),
              model(std::move(other.model)) {}

    ClientHandler::~ClientHandler() {
        close(); //this may actually throw on attempt to lock mutex...
        th.join();
    }

    void ClientHandler::sync(const Board& board) {
        model.sync(board);
    }

    PlayerReply ClientHandler::sendRequest(Player& p, PlayerRequest request) {
        return model.processRequest(std::move(request));
    }

    void ClientHandler::sendMessage(Player& p, PlayerMessage message) {
        model.processMessage(std::move(message));
    }

    void ClientHandler::close() {
        std::lock_guard g(socketMutex);
        socket.close();
    }

    void ClientHandler::mainLoop() {
        using Messages::MessageType;
        while (true) {
            std::lock_guard g(socketMutex);
            if (!socket.is_open()) break;
            if (model.isDirty())
                Messages::send(socket, MessageType::BOARD,
                               Serialization::serializeBoard(model.getBoard()));
            if (auto req = model.getRequest(); req.has_value())
                Messages::send(socket, MessageType::REQUEST,
                               Serialization::serializeRequest(req.value()));
            if (auto msg = model.getMessage(); msg.has_value())
                Messages::send(socket, MessageType::MESSAGE,
                               Serialization::serializeMessage(msg.value()));
            if (auto clientMsg = Messages::receive(socket, true); clientMsg.has_value()) {
                const auto&[type, content] = clientMsg.value();
                switch (type) {
                    case MessageType::REPLY: {
                        model.sendReply(Serialization::deserializeReply(content));
                        break;
                    }
                    default:
                        throw Messages::InvalidMessageError("Unexpected message type for c2s message");
                }
            }
        }
        onDeath(id);
    }

    ClientId ClientHandler::getId() const {
        return id;
    }

    void ClientHandler::start() {
        th = std::thread([this]() { this->start(); });
    }

    ConnectionAcceptor::~ConnectionAcceptor() {
        close();
        th.join();
    }

    void ConnectionAcceptor::close() {
        std::lock_guard g(acceptorMutex);
        acceptor.close();
    }

    void ConnectionAcceptor::mainLoop() {
        while (true) {
            std::lock_guard g(acceptorMutex);
            if (!acceptor.is_open()) break;
            sockpp::inet_address addr;
            auto socket = acceptor.accept(&addr);
            ClientId id = curId++;
            clients.emplace(id, ClientHandler(id, addr, std::move(socket),
                                              [this](ClientId id) { this->clients.erase(id); }));
            auto& handler = clients.at(id);
            handler.start();
        }
    }
}