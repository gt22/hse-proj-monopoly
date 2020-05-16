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
    for (size_t i = 0; i < arg.availableActions.size(); ++i) {
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



#undef SER_SPEC