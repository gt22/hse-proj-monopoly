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

add_subdirectory(lib)

file(GLOB_RECURSE sources "src/*")
file(GLOB_RECURSE includes "include/*")

add_executable(hseproj ${sources} ${includes} ${ProtoSources} ${ProtoHeaders})
target_link_libraries(hseproj Threads::Threads sfml-graphics ${PROTOBUF_LIBRARIES} ${SOCKPP})

include(cmake/googletest.cmake)
fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest
)

add_library(hseproj-lib "")
target_link_libraries(hseproj-lib sfml-graphics)


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
