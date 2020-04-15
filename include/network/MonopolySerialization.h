//
// Created by Igor Engel on 03.04.2020.
//

#ifndef HSEPROJ_MONOPOLYSERIALIZATION_H
#define HSEPROJ_MONOPOLYSERIALIZATION_H

#include "MonopolyFwd.h"
namespace Monopoly::Serialization {

    std::string serializeRequest(const PlayerRequest& req);
    PlayerRequest deserializeRequest(const std::string& ser);

    std::string serializeMessage(const PlayerMessage& msg);
    PlayerMessage deserializeMessage(const std::string& ser);

    std::string serializeReply(const PlayerReplyData& rep);
    PlayerReply deserializeReply(const std::string& ser);

    std::string serializeBoard(const Board& board);
    Board deserializeBoard(const std::string& ser, Game& game);

}

#endif //HSEPROJ_MONOPOLYSERIALIZATION_H
