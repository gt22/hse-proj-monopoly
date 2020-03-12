#ifndef MONOPOLYFWD_H
#define MONOPOLYFWD_H

#include <cstddef>
#include <memory>

struct PlayerData;
class Board;

class Card;
class CardPool;

class FieldTile;
class OwnableTile;
class Start;
class Prison;
class Chance;

class Game;

class Manager;

enum class Token : size_t;
struct PlayerRequest;
struct PlayerReply;
class Player;
class NetworkPlayer;
class BotPlayer;
class LocalPlayer;

class AbstractView;
class MenuView;
class MonopolyView;
class NcursesView;
using ViewHolder = std::shared_ptr<AbstractView>;

#endif //MONOPOLYFWD_H
