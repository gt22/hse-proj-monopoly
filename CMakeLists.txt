cmake_minimum_required(VERSION 3.15)
project(hseproj)

add_compile_options("-Wall")
add_compile_options("-Wextra")
add_compile_options("-Werror")
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(hseproj src/main.cpp include/View.h src/View.cpp src/Player.cpp include/Player.h src/Board.cpp include/Board.h src/Manager.cpp include/Manager.h include/Field.h src/Field.cpp include/MonopolyFwd.h src/Game.cpp include/Game.h include/Cards.h include/PlayerRequests.h)
target_link_libraries(hseproj ${CURSES_LIBRARIES})


