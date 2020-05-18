#ifndef MONOPOLYFWD_H
#define MONOPOLYFWD_H

#include <cstddef>
#include <memory>
#include <map>
//#include "View.h"

struct PlayerData;
class Board;

class Card;
class CardPool;

class FieldTile;
class OwnableTile;
class Start;
class Prison;
class GoToPrison;
class Chance;
class IncomeTax;
class Railway;
class Street;
class Utility;
class FreeParking;
enum class Color : std::size_t;

class Game;

class Manager;

enum class Token : std::size_t {
    DOG,
    HAT,
    BOOT,
    CAT,
    CAR,
    SHIP,
    FREE_FIELD
};

enum class TokenForMortgage : std::size_t {
    DOG_M,
    HAT_M,
    BOOT_M,
    CAT_M,
    CAR_M,
    SHIP_M,
};

enum class MessageType : std::size_t {
    INFO,
    CHANCE,
    PUBLIC_TREASURY,
    DICE
};

class Player;
class NetworkPlayer;
class BotPlayer;
class LocalPlayer;

struct PlayerRequest;
struct PlayerReplyData;
struct NumReplyData;
struct PlayerMessage;
struct PlayerTradeReplyData;
struct PlayerTradeRequest;
struct SumReplyData;
typedef std::unique_ptr<PlayerReplyData> PlayerReply;
typedef std::unique_ptr<NumReplyData> NumReply;
typedef std::unique_ptr<SumReplyData> SumReply;
typedef std::unique_ptr<SumReplyData> TokenReply;
typedef std::unique_ptr<PlayerTradeReplyData> PlayerTradeReply;

class RandomSource;

class AbstractView;
class MenuView;
class MonopolyView;
class NcursesView;
using ViewHolder = std::unique_ptr<AbstractView>;

class SFMLView;

class MonopolyThreadManager;

#endif //MONOPOLYFWD_H
