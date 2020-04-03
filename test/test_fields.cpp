#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>

#include "Field.h"
#include "Manager.h"


using namespace testing;


class MockPlayerData : public PlayerData {
public:
    MockPlayerData() : PlayerData("", Token::CAR) {}
    MockPlayerData(const MockPlayerData& data) : MockPlayerData() {}
};


class MockBoard : public Board {
    Manager manager;
    Game game = Game({}, manager);

public:
    MockBoard() : Board({}, game) {}

    MOCK_METHOD(PlayerData&, getPlayer, (Token token), (override));
    MOCK_METHOD(PlayerReply, sendRequest, (Token token, PlayerRequest request), (const, override));
};


MATCHER_P(PlayerRequestMatcher, other, "") {
    return std::set<PlayerAction>(arg.availableActions.begin(), arg.availableActions.end()) == \
            std::set<PlayerAction>(other.availableActions.begin(), other.availableActions.end()) \
            && arg.message == other.message;
}


TEST(TestFields, TestStartEntry) {
    StrictMock<MockPlayerData> playerData;

    StrictMock<MockBoard> board;
    EXPECT_CALL(board, getPlayer(Token::CAR)).WillOnce(ReturnRef(playerData));

    PlayerRequest expectedRequest(
            {PlayerAction::BUY_BUILDING, PlayerAction::BUY_HOTEL, PlayerAction::USE_CARD,
             PlayerAction::MORTGAGE_HOLDINGS, PlayerAction::START_TRADE, PlayerAction::END_TURN},
            "");

    EXPECT_CALL(board, sendRequest(Token::CAR, PlayerRequestMatcher(expectedRequest))).WillOnce(Return(ByMove(
            std::make_unique<PlayerReplyData>(PlayerReplyData(PlayerAction::END_TURN)))));

    Start(board, 0, "").onPlayerEntry(Token::CAR);
}
