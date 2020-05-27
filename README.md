# Проект "Монополия" [![Build Status](https://travis-ci.org/gt22/hse-proj-monopoly.svg?branch=master)](https://travis-ci.org/gt22/hse-proj-monopoly)
TODO

Необходимые библеотеки:
* SFML 2.5.1
* sockpp commit 93855d5 (добавлена как submodule)
* protobuf 3.11.4


Процесс сборки:
```shell script
git clone https://github.com/gt22/hse-proj-monopoly
cd hse-proj-monopoly
mkdir build
cd build
cmake ..
make
```

После этого, можно (находясь в директории) `build` запустить проект используя `./monopoly`