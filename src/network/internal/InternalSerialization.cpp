#include "monopoly.pb.h"
#include "network/internal/InternalSerialization.h"
#include "PlayerRequests.h"
#include "Field.h"
#include "Cards.h"
#include "Board.h"
namespace Monopoly::Serialization::Internal {

    Pb::PlayerRequest serializeRequest(const PlayerRequest& req) {
        Pb::PlayerRequest ser;
        for(auto action : req.availableActions) {
            ser.add_availableactions(Pb::PlayerAction(action));
        }
        ser.set_msg(req.message);
        return ser;
    }

    PlayerRequest deserializeRequest(const Pb::PlayerRequest& ser) {
        PlayerRequest ret;
        ret.availableActions.reserve(ser.availableactions_size());
        for(auto action : ser.availableactions()) {
            ret.availableActions.push_back(PlayerAction(action));
        }
        ret.message = ser.msg();
        return ret;
    }

    Pb::PlayerMessage serializeMessage(const PlayerMessage& msg) {
        Pb::PlayerMessage ser;
        ser.set_msg(msg.message);
        return ser;
    }

    PlayerMessage deserializeMessage(const Pb::PlayerMessage& ser) {
        return PlayerMessage(ser.msg());
    }

    Pb::PlayerReply serializeReply(const PlayerReplyData& rep) {
        Pb::PlayerReply ser; //TODO: Subclasses
        ser.set_action(Pb::PlayerAction(rep.action));
        return ser;
    }

    PlayerReply deserializeReply(const Pb::PlayerReply& ser) {
        //TODO: Subclasses
        return std::make_unique<PlayerReplyData>(PlayerAction(ser.action()));
    }

    Pb::FieldTile serializeFieldTile(const FieldTile& tile) {
        Pb::FieldTile ser;
        ser.set_type(Pb::FieldTile_TileType(tile.type));
        ser.set_position(tile.position);
        ser.set_name(tile.name);
        try {
            switch (tile.type) {
                case TileType::START:
                    break;
                case TileType::OWNABLE_TILE: {
                    const auto &ownable = dynamic_cast<const OwnableTile &>(tile);
                    auto ownableSer = ser.mutable_ownabletile();
                    ownableSer->set_ownabletype(Pb::FieldTile_OwnableTile_OwnableType(ownable.ownableType));
                    ownableSer->set_cost(ownable.cost);
                    ownableSer->set_cost_of_parking(ownable.costOfParking);
                    ownableSer->set_color(Pb::Color(ownable.color));
                    ownableSer->set_owner(Pb::Token(ownable.owner));
                    switch (ownable.ownableType) {
                        case OwnableType::RAILWAY:
                            break;
                        case OwnableType::STREET: {
                            const auto &street = dynamic_cast<const Street &>(ownable);
                            auto streetSer = ownableSer->mutable_street();
                            streetSer->set_number_of_houses(street.numberOfHouses);
                            streetSer->set_cost_per_house(street.costPerHouse);
                            break;
                        }
                        case OwnableType::UTILITY:
                            break;
                        case OwnableType::UNINITIALIZED:
                            assert(false); //Attempt to serialize uninitialized tile
                    }
                    break;
                }
                case TileType::PRISON:
                case TileType::GOTO_PRISON:
                case TileType::CHANCE:
                    break;
                case TileType::INCOME_TAX: {
                    const auto &itax = dynamic_cast<const IncomeTax &>(tile);
                    ser.mutable_incometax()->set_tax(itax.tax);
                    break;
                }
                case TileType::FREE_PARKING:
                    break;
                case TileType::UNINITIALIZED:
                    assert(false); //Attempt to serialize uninitialized tile
            }
        } catch(std::bad_cast& e) {
            assert(false); //TODO: exception handling?
        }
        return ser;
    }

    FieldTile* deserializeFieldTile(const Pb::FieldTile& ser, Board& board) {
        FieldTile *ret;
        switch(TileType(ser.type())) {

            case TileType::START: ret = new Start(board); break;
            case TileType::OWNABLE_TILE: {
                const auto& ownableSer = ser.ownabletile();
                OwnableTile *ownable;
                switch(OwnableType(ownableSer.ownabletype())) {
                    case OwnableType::RAILWAY: ret = ownable = new Railway(board); break;
                    case OwnableType::STREET: {
                        const auto& streetSer = ownableSer.street();
                        auto street = new Street(board);
                        ret = ownable = street;
                        street->numberOfHouses = streetSer.number_of_houses();
                        street->costPerHouse = streetSer.cost_per_house();
                        break;
                    }
                    case OwnableType::UTILITY: ret = ownable = new Utility(board); break;
                    case OwnableType::UNINITIALIZED: assert(false);
                }
                ownable->ownableType = OwnableType(ownableSer.ownabletype());
                ownable->cost = ownableSer.cost();
                ownable->costOfParking = ownableSer.cost_of_parking();
                ownable->color = Color(ownableSer.color());
                ownable->owner = Token(ownableSer.owner());
                break;
            }
            case TileType::PRISON: ret = new Prison(board); break;
            case TileType::GOTO_PRISON: ret = new GoToPrison(board); break;
            case TileType::CHANCE: ret = new Chance(board); break;
            case TileType::INCOME_TAX: {
                auto itax = new IncomeTax(board);
                ret = itax;
                itax->tax = ser.incometax().tax();
                break;
            }
            case TileType::FREE_PARKING: ret = new FreeParking(board); break;
            case TileType::UNINITIALIZED: assert(false);
        }
        ret->type = TileType(ser.type());
        ret->position = ser.position();
        ret->name = ser.name();
        return ret;
    }

    Pb::Card serializeCard(const Card& card) {
        Pb::Card ser;
        ser.set_text(card.text);
        return ser;
    }

    Card* deserializeCard(const Pb::Card& data, Board&) {
        //TODO: Subclasses
        throw 1;
    }

    Pb::PlayerData serializePlayerData(const PlayerData& data) {
        Pb::PlayerData ser;
        ser.set_name(std::string(data.name));
        ser.set_token(Pb::Token(data.token));
        ser.set_position(data.position);
        ser.set_money(data.money);
        ser.set_doubledice(data.doubleDice);
        ser.set_daysleftinprison(data.daysLeftInPrison);
        ser.set_numberofrailways(data.numberOfRailways);
        ser.set_numberofutilities(data.numberOfUtilities);
        ser.set_lastthrow(data.lastTrow);
        ser.set_prisoner(data.prisoner);
        for(const auto& card : data.cards) {
            *ser.add_card() = serializeCard(*card);
        }
        return ser;
    }



    PlayerData deserializePlayerData(const Pb::PlayerData& ser, Board& board) {
        PlayerData ret(ser.name(), Token(ser.token()));
        ret.position = ser.position();
        ret.money = ser.money();
        ret.doubleDice = ser.doubledice();
        ret.daysLeftInPrison = ser.daysleftinprison();
        ret.numberOfRailways = ser.numberofrailways();
        ret.numberOfUtilities = ser.numberofutilities();
        ret.lastTrow = ser.lastthrow();
        ret.prisoner = ser.prisoner();
        for(const auto& card : ser.card()) {
            ret.cards.emplace_back(deserializeCard(card, board));
        }
        return ret;
    }

}