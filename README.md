# Проект "Монополия" 
## О проекте
### Основная идея
«Монополия» — экономическая и стратегическая настольная игра для 2-6 человек.

Цель игры — рационально используя стартовый капитал, добиться банкротства других игроков.

Подробнее узнать об игре, а также ознакомиться с правилами, вы можете [здесь](https://www.keft.ru/help/mnp/rules).

### Проект
Данный проект - реализация Монополии в виде компьютерной игры.

Текущая версия проекта представляет собой приложение, написанное на **SFML**. 
## Установка
Операционная система: **OS Linux** или **MacOS**
### Необходимые библеотеки
* SFML 2.5.1
 ```
    $ sudo apt-get install libsfml-dev
 ```
* sockpp commit 93855d5 (добавлена как submodule)
 ```
    $ git submodule update --init --recursive
 ```
* [protobuf 3.11.4](https://github.com/protocolbuffers/protobuf/releases/download/v3.11.4/protobuf-cpp-3.11.4.tar.gz)

### Сборка и использование
```shell script
git clone https://github.com/gt22/hse-proj-monopoly
cd hse-proj-monopoly
mkdir build
cd build
cmake ..
make
```

После этого, можно (находясь в директории `build`) запустить проект используя `./monopoly`


## Авторы
* https://github.com/gt22
* https://github.com/xbreathoflife
* https://github.com/AlinaUsh
