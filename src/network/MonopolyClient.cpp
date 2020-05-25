#include "network/MonopolyClient.h"
#include "network/internal/MessageFormat.h"
#include "network/MonopolySerialization.h"
namespace Monopoly::Network {


    Client::Client(const sockpp::inet_address& addr, sockpp::tcp_socket socket,
                   Threads::ModelThreadManager& model, std::function<void()> onDeath)
                   : addr(addr), socket(std::move(socket)), model(model), onDeath(std::move(onDeath)) {}

    void Client::mainLoop() {
        using Messages::MessageType;
        while (true) {
            std::lock_guard g(socketMutex);
            if (!socket.is_open()) break;
            if(auto rep = model.getReply(); rep.has_value())
                Messages::send(socket, MessageType::REPLY, Serialization::serializeReply(rep.value()));
            if(auto msg = Messages::receive(socket, true); msg.has_value()) {
                const auto&[type, content] = msg.value();
                switch(type) {
                    case MessageType::BOARD: {
                        model.sync(Serialization::deserializeBoard(content));
                        break;
                    }
                    case MessageType::REQUEST: {
                        model.processRequestAsync(Serialization::deserializeRequest(content));
                        break;
                    }
                    default: throw Messages::InvalidMessageError("Unexpected message type for s2c message");
                }
            }
        }
    }
}