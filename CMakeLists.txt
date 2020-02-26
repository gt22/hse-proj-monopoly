cmake_minimum_required(VERSION 3.15)
project(hseproj)

add_compile_options("-Wall")
add_compile_options("-Wextra")
add_compile_options("-Werror")
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(hseproj src/main.cpp include/view.h src/view.cpp)
target_link_libraries(hseproj ${CURSES_LIBRARIES})