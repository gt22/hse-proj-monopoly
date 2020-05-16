//
// Created by Igor Engel on 30.03.2020.
//

#ifndef HSEPROJ_INTERNALSERIALIZATION_H
#define HSEPROJ_INTERNALSERIALIZATION_H

#include "MonopolyFwd.h"
#include "monopoly.pb.h"
namespace Monopoly::Serialization::Internal {

    Pb::PlayerRequest serializeRequest(const PlayerRequest& req);
    PlayerRequest deserializeRequest(const Pb::PlayerRequest& ser);

    Pb::PlayerMessage serializeMessage(const PlayerMessage& msg);
    PlayerMessage deserializeMessage(const Pb::PlayerMessage& ser);

    Pb::PlayerReply serializeReply(const PlayerReplyData& rep);
    PlayerReply deserializeReply(const Pb::PlayerReply& ser);

    Pb::FieldTileModel serializeFieldTile(const FieldTileModel& data);
    FieldTileModel deserializeFieldTile(const Pb::FieldTileModel& ser);

    Pb::Card serializeCard(const Card& card);
    Card* deserializeCard(const Pb::Card& data, Board&);

    Pb::PlayerData serializePlayerModel(const PlayerModel& data);
    PlayerModel deserializePlayerModel(const Pb::PlayerData& ser);

    Pb::BoardModel serializeBoardModel(const BoardModel& board);
    BoardModel deserializeBoardModel(const Pb::BoardModel& ser);

}

#endif //HSEPROJ_INTERNALSERIALIZATION_H
