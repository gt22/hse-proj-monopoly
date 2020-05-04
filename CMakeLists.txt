cmake_minimum_required(VERSION 3.15)
project(hseproj)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-unused-private-field)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_compile_options("-fsanitize=address")
add_link_options(-fsanitize=address)
#set(CMAKE_CXX_FLAGS "-lncurses")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

add_executable(hseproj src/main.cpp include/View.h src/View.cpp src/Player.cpp include/Player.h src/Board.cpp include/Board.h src/Manager.cpp include/Manager.h include/Field.h src/Field.cpp include/MonopolyFwd.h src/Game.cpp include/Game.h include/Cards.h include/PlayerRequests.h src/Cards.cpp include/Random.h src/Random.cpp src/PlayerRequesrs.cpp src/Menu.cpp include/Menu.h include/view/SFMLView.h src/view/SFMLView.cpp include/MonopolyThreadManager.h src/MonopolyThreadManager.cpp include/view/EventManager.h src/view/EventManager.cpp include/view/VectorOps.h include/view/BoardModel.h src/view/BoardModel.cpp include/view/ButtonManager.h src/view/ButtonManager.cpp)
target_link_libraries(hseproj ${CURSES_LIBRARIES} Threads::Threads sfml-graphics)

include(cmake/googletest.cmake)
fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest
)

add_library(hseproj-lib "")
target_link_libraries(hseproj-lib ${CURSES_LIBRARIES})
file(GLOB sources "src/*")
file(GLOB includes "include/*")

target_sources(
        hseproj-lib
        PRIVATE
            ${sources}
        PUBLIC
            ${include}
)

target_include_directories(
        hseproj-lib
        PUBLIC
            include
)

enable_testing()
add_subdirectory(test)
