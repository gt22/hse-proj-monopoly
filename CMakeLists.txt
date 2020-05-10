cmake_minimum_required(VERSION 3.15)
project(hseproj)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-unused-private-field)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
include_directories(include)

add_compile_options(-fsanitize=undefined,thread)
add_link_options(-fsanitize=undefined,thread)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)
find_package(Protobuf REQUIRED)
file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(hseproj src/main.cpp include/View.h src/Player.cpp include/Player.h src/Board.cpp include/Board.h src/Manager.cpp include/Manager.h include/Field.h src/Field.cpp include/MonopolyFwd.h src/Game.cpp include/Game.h include/Cards.h include/PlayerRequests.h src/Cards.cpp include/Random.h src/Random.cpp src/PlayerRequesrs.cpp include/view/SFMLView.h src/view/SFMLView.cpp include/MonopolyThreadManager.h src/MonopolyThreadManager.cpp include/view/EventManager.h src/view/EventManager.cpp include/view/VectorOps.h include/view/BoardModel.h src/view/BoardModel.cpp include/view/ButtonManager.h src/view/ButtonManager.cpp include/view/TooltipManager.h src/view/TooltipManager.cpp include/network/internal/InternalSerialization.h ${ProtoSources} ${ProtoHeaders} src/network/internal/InternalSerialization.cpp include/network/MonopolySerialization.h src/network/MonopolySerialization.cpp)
target_link_libraries(hseproj Threads::Threads sfml-graphics ${PROTOBUF_LIBRARIES})

include(cmake/googletest.cmake)
fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest
)

add_library(hseproj-lib "")
target_link_libraries(hseproj-lib sfml-graphics)
file(GLOB_RECURSE sources "src/*")
file(GLOB_RECURSE includes "include/*")

target_sources(
        hseproj-lib
        PRIVATE
            ${sources}
            ${ProtoHeaders}
            ${ProtoSources}
        PUBLIC
            ${include}
)
target_link_libraries(
        hseproj-lib
        ${PROTOBUF_LIBRARIES}
)

target_include_directories(
        hseproj-lib
        PUBLIC
            include
)

enable_testing()
add_subdirectory(test)
