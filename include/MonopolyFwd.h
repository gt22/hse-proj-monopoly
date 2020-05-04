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

class Player;
class NetworkPlayer;
class BotPlayer;
class LocalPlayer;

struct PlayerRequest;
struct PlayerReplyData;

class RandomSource;

class AbstractView;
class MenuView;
class MonopolyView;
class NcursesView;
using ViewHolder = std::unique_ptr<AbstractView>;

#endif //MONOPOLYFWD_H
