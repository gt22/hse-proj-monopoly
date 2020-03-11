#ifndef MONOPOLYFWD_H
#define MONOPOLYFWD_H

#include <cstddef>

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

enum class Token : size_t;
struct PlayerRequest;
struct PlayerReply;
class Player;
class NetworkPlayer;
class BotPlayer;
class LocalPlayer;

#endif //MONOPOLYFWD_H
