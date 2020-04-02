cmake_minimum_required(VERSION 3.15)
project(hseproj)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-unused-private-field)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
include_directories(include)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Protobuf REQUIRED)
file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(hseproj src/main.cpp include/View.h src/View.cpp src/Player.cpp include/Player.h src/Board.cpp include/Board.h src/Manager.cpp include/Manager.h include/Field.h src/Field.cpp include/MonopolyFwd.h src/Game.cpp include/Game.h include/Cards.h include/PlayerRequests.h src/Cards.cpp include/Random.h src/Random.cpp src/PlayerRequesrs.cpp src/Menu.cpp include/Menu.h include/network/internal/InternalSerialization.h ${ProtoSources} ${ProtoHeaders} src/network/internal/InternalSerialization.cpp)
target_link_libraries(hseproj ${CURSES_LIBRARIES} ${PROTOBUF_LIBRARIES})

