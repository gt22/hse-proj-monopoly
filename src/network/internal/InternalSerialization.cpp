#include "network/internal/InternalSerialization.h"
#include "PlayerRequests.h"
#include "Field.h"
#include "Cards.h"
#include "Board.h"
#include "include/threads/BoardModel.h"
#include <google/protobuf/port_def.inc>

namespace Monopoly::Serialization::Internal {

    Pb::PlayerRequest serializeRequest(const PlayerRequest& req) {
        Pb::PlayerRequest ser;
        for (auto action : req.availableActions) {
            ser.add_availableactions(Pb::PlayerAction(action));
        }
        ser.set_msg(req.message);
        return ser;
    }

    PlayerRequest deserializeRequest(const Pb::PlayerRequest& ser) {
        PlayerRequest ret;
        ret.availableActions.reserve(ser.availableactions_size());
        for (auto action : ser.availableactions()) {
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

    template<typename T, typename U>
    std::optional<U> apply(std::optional<T> inp, std::function<U(const T&)> func) {
        if (inp.has_value()) return func(inp.value());
        else return {};
    }

    Pb::Taxes serializeTaxes(const Taxes& data) {
        Pb::Taxes ser;
        ser.set_starttax(data.startTax);
        ser.set_taxonehouse(data.taxOneHouse);
        ser.set_taxtwohouses(data.taxTwoHouses);
        ser.set_taxthreehouses(data.taxThreeHouses);
        ser.set_taxfourhouses(data.taxFourHouses);
        ser.set_taxhotel(data.taxHotel);
        return ser;
    }

    Taxes deserializeTaxes(const Pb::Taxes& ser) {
        Taxes ret;
        ret.startTax = ser.starttax();
        ret.taxOneHouse = ser.taxonehouse();
        ret.taxTwoHouses = ser.taxtwohouses();
        ret.taxThreeHouses = ser.taxthreehouses();
        ret.taxFourHouses = ser.taxfourhouses();
        ret.taxHotel = ser.taxhotel();
        return ret;
    }

    template<typename I, typename O>
    std::function<O(const I&)> converter() {
        return [](const I& x) { return O(x); };
    }

    Pb::FieldTileModel serializeFieldTile(const FieldTileModel& data) {
        Pb::FieldTileModel ser;
        ser.set_position(data.position);
        ser.set_name(data.name);
        ser.set_tax(data.tax.value_or(-1));
        ser.set_cost(data.cost.value_or(-1));
        ser.set_costofparking(data.costOfParking.value_or(-1));
        ser.set_color(apply(
                data.color, converter<Color, Pb::Color>()
        ).value_or(Pb::Color::UNSPECIFIED_col));
        ser.set_owner(apply(
                data.owner, converter<Token, Pb::Token>()
        ).value_or(Pb::Token::UNSPECIFIED_tok));
        ser.set_numberofhouses(data.numberOfHouses.value_or(-1));
        ser.set_costperhouse(data.costPerHouse.value_or(-1));
        if (data.taxes.has_value()) {
            *ser.mutable_taxes() = serializeTaxes(data.taxes.value());
        }
        return ser;
    }

    template<typename T>
    std::optional<T> loadIfPresent(const T& value, const T& notPresent) {
        if (value == notPresent) return {};
        else return value;
    }

    FieldTileModel deserializeFieldTile(const Pb::FieldTileModel& ser) {
        FieldTileModel ret;
        ret.position = ser.position();
        ret.name = ser.name();
        ret.tax = loadIfPresent(ser.tax(), -1);
        ret.cost = loadIfPresent(ser.cost(), -1);
        ret.costOfParking = loadIfPresent(ser.costofparking(), -1);
        ret.color = apply(loadIfPresent(ser.color(), Pb::Color::UNSPECIFIED_col),
                          converter<Pb::Color, Color>());
        ret.owner = apply(loadIfPresent(ser.owner(), Pb::Token::UNSPECIFIED_tok),
                          converter<Pb::Token, Token>());
        ret.numberOfHouses = loadIfPresent(ser.numberofhouses(), -1);
        ret.costPerHouse = loadIfPresent(ser.costperhouse(), -1);
        if(ser.has_taxes()) ret.taxes = deserializeTaxes(ser.taxes());
        return ret;
    }

    Pb::Card serializeCard(const Card& card) {
        Pb::Card ser;
        ser.set_text(card.text);
        return ser;
    }

    Card *deserializeCard(const Pb::Card& data, Board&) {
        //TODO: Subclasses
        throw 1;
    }

    Pb::PlayerData serializePlayerModel(const PlayerModel& data) {
        Pb::PlayerData ser;
        ser.set_name(data.name);
        ser.set_token(Pb::Token(data.token));
        ser.set_position(data.position);
        ser.set_money(data.money);
        ser.set_prisoner(data.prisoner);
        ser.set_alive(data.alive);
        return ser;
    }

    PlayerModel deserializePlayerModel(const Pb::PlayerData& ser) {
        PlayerModel ret;
        ret.name = ser.name();
        ret.token = Token(ser.token());
        ret.position = ser.position();
        ret.money = ser.money();
        ret.prisoner = ser.prisoner();
        ret.alive = ser.alive();
        return ret;
    }

    template<typename T, typename C, typename F>
    //T - serialized type
    //C - container of type V
    void addAll(PROTOBUF_NAMESPACE_ID::RepeatedPtrField<T> *field, const C& cont, F conv) {
        field->Reserve(cont.size());
        for (const auto& x : cont) {
            field->Add(conv(x));
        }
    }

    Pb::BoardModel serializeBoardModel(const BoardModel& board) {
        Pb::BoardModel ser;
        addAll(ser.mutable_tiles(), board.field, serializeFieldTile);
        addAll(ser.mutable_players(), board.players, serializePlayerModel);
        ser.set_curplayer(Pb::Token(board.curPlayer));
        return ser;
    }

    BoardModel deserializeBoardModel(const Pb::BoardModel& ser) {
        BoardModel ret;
        for(const auto& tileSer : ser.tiles()) {
            auto tile = deserializeFieldTile(tileSer);
            ret.field[tile.position] = std::move(tile);
        }
        ret.players.reserve(ser.players_size());
        for (const auto& dataSer : ser.players()) {
            ret.players.push_back(deserializePlayerModel(dataSer));
        }
        ret.curPlayer = Token(ser.curplayer());
        return ret;
    }
}