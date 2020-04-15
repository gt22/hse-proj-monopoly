#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>
#include <Field.h>

#include "network/MonopolySerialization.h"
#include "PlayerRequests.h"
#include "Board.h"

using namespace testing;
namespace Ser = Monopoly::Serialization;

template<typename T>
struct TestSpec;

#define SER_SPEC(T, SER, DESER, MATCH) template<>\
struct TestSpec<T> { \
    constexpr static const auto& ser = SER; \
    constexpr static const auto& deser = DESER; \
    constexpr static const auto& matcher = MATCH<const T&>; \
}

template<typename T>
void check(T origin) {
    using Spec = TestSpec<T>;
    T deserialized = Spec::deser(Spec::ser(origin));
    EXPECT_THAT(deserialized, Spec::matcher(origin));
}

MATCHER_P(PlayerRequestMatcher, other, "") {
    if(arg.message != other.message || arg.availableActions.size() != other.availableActions.size()) {
        return false;
    }
    for (int i = 0; i < arg.availableActions.size(); ++i) {
        if(arg.availableActions[i] != other.availableActions[i]) {
            return false;
        }
    }
    return true;
}

SER_SPEC(PlayerRequest, Ser::serializeRequest, Ser::deserializeRequest, PlayerRequestMatcher);

TEST(TestSerialization, TestRequest) {
    check(PlayerRequest({ PlayerAction::END_TURN,
                          PlayerAction::ROLL_DICE,
                          PlayerAction::PAY_TAX }, "asdфыв"));
}


MATCHER_P(PlayerMessageMatcher, other, "") {
    return arg.message == other.message;
}

SER_SPEC(PlayerMessage, Ser::serializeMessage, Ser::deserializeMessage, PlayerMessageMatcher);

TEST(TestSerialization, TestMessage) {
    check(PlayerMessage("asd"));
    check(PlayerMessage("фыв"));
}

//Call function taking reference from pointer (or pointer-like) argument
// F :: T& -> R
// dereferenceCall<F> :: T* -> R
template<typename F>
constexpr auto dereferenceCall(F func) {
    return [func](const auto& x) { return func(*x); };
}

MATCHER_P(PlayerReplyMatcher, other, "") {
    return arg->action == other->action;
}

SER_SPEC(PlayerReply, dereferenceCall(Ser::serializeReply), Ser::deserializeReply, PlayerReplyMatcher);

TEST(TestSerialization, TestReply) {
    check(std::make_unique<PlayerReplyData>(PlayerAction::END_TURN));
    check(std::make_unique<PlayerReplyData>(PlayerAction::PAY_TO_OTHER_PLAYER));
}


bool matchTile(const FieldTile* a, const FieldTile* b) {
    if(a->type != b->type) return false;
    if(a->name != b->name || a->position != b->position) return false;
    switch(a->type) {
        case TileType::START: return dynamic_cast<const Start*>(a) && dynamic_cast<const Start*>(b);
        case TileType::OWNABLE_TILE: {
            auto ao = dynamic_cast<const OwnableTile*>(a), bo = dynamic_cast<const OwnableTile*>(b);
            if(!ao || !bo || ao->type != bo->type) return false;
            switch(ao->ownableType) {
                case OwnableType::RAILWAY: return dynamic_cast<const Railway*>(ao) && dynamic_cast<const Railway*>(bo);
                case OwnableType::STREET: {
                    auto as = dynamic_cast<const Street*>(ao), bs = dynamic_cast<const Street*>(bo);
                    return as && bs && as->numberOfHouses == bs->numberOfHouses && as->costPerHouse == bs->costPerHouse;
                }
                case OwnableType::UTILITY: return dynamic_cast<const Utility*>(ao) && dynamic_cast<const Utility*>(bo);
                case OwnableType::UNINITIALIZED: assert(false);
            }
            break;
        }
        case TileType::PRISON: return dynamic_cast<const Prison*>(a) && dynamic_cast<const Prison*>(b);
        case TileType::GOTO_PRISON: return dynamic_cast<const GoToPrison*>(a) && dynamic_cast<const GoToPrison*>(b);
        case TileType::CHANCE: return dynamic_cast<const Chance*>(a) && dynamic_cast<const Chance*>(b);
        case TileType::INCOME_TAX: {
            auto ai = dynamic_cast<const IncomeTax*>(a), bi = dynamic_cast<const IncomeTax*>(b);
            return ai && bi && ai->tax == bi->tax;
        }
        case TileType::FREE_PARKING: return dynamic_cast<const FreeParking*>(a) && dynamic_cast<const FreeParking*>(b);
        case TileType::UNINITIALIZED: assert(false);
    }
    return true;
}

bool matchCard(const Card& a, const Card& b) {
    return true; //TODO
}

bool matchPlayerData(const PlayerData& a, const PlayerData& b) {
    if( a.name              != b.name              ||
        a.token             != b.token             ||
        a.position          != b.position          ||
        a.money             != b.money             ||
        a.doubleDice        != b.doubleDice        ||
        a.daysLeftInPrison  != b.daysLeftInPrison  ||
        a.numberOfRailways  != b.numberOfRailways  ||
        a.numberOfUtilities != b.numberOfUtilities ||
        a.lastTrow          != b.lastTrow          ||
        a.prisoner          != b.prisoner          ||
        a.alive             != b.alive             ||
        a.cards.size()      != b.cards.size()) {
        return false;
    }
    for (size_t i = 0; i < a.cards.size(); ++i) {
        if(!matchCard(*a.cards[i], *b.cards[i])) {
            return false;
        }
    }
    return true;
}

MATCHER_P(BoardMatcher, other, "") {
    if(arg.field.size()           != other.field.size()     ||
       arg.players.size()         != other.players.size()   ||
       arg.deck.getCards().size() != other.deck.getCards().size()) {
        return false;
    }
    for (size_t i = 0; i < arg.field.size(); i++) {
        if(!matchTile(arg.field[i], other.field[i])) {
            return false;
        }
    }
    for (size_t i = 0; i < arg.players.size(); ++i) {
        if(!matchPlayerData(arg.players[i], other.players[i])) {
            return false;
        }
    }
    for (size_t i = 0; i < arg.deck.getCards(); ++i) {
        if(!matchCard(arg.deck.getCards()[i], other.deck.getCards()[i])) {
            return false;
        }
    }
    return true;
}

SER_SPEC(Board, Ser::serializeBoard, Ser::deserializeBoard, BoardMatcher);

TEST(TestSerialization, TestBoard) {
    //TODO
}

#undef SER_SPEC