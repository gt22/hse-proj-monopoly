#ifndef MONOPOLYFWD_H
#define MONOPOLYFWD_H

#include <cstddef>
#include <memory>
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

class Player;
class NetworkPlayer;
class BotPlayer;
class LocalPlayer;

struct PlayerRequest;
struct PlayerReply;

class RandomSource;

class AbstractView;
class MenuView;
class MonopolyView;
class NcursesView;
using ViewHolder = std::shared_ptr<AbstractView>;

#endif //MONOPOLYFWD_H
