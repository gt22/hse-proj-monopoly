cmake_minimum_required(VERSION 3.15)
project(monopoly)

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
find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(lib)

file(GLOB_RECURSE sources "src/*")
file(GLOB_RECURSE includes "include/*")

add_executable(${PROJECT_NAME} ${sources} ${includes} ${ProtoSources} ${ProtoHeaders})

set (resources "${CMAKE_SOURCE_DIR}/resources")
set (resources_target "${CMAKE_CURRENT_BINARY_DIR}/resources")
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${resources} ${resources_target}
        DEPENDS ${images_target}
        COMMENT "symbolic link resources folder from ${resources} => ${resources_target}"
)

target_link_libraries(${PROJECT_NAME} Threads::Threads sfml-graphics ${PROTOBUF_LIBRARIES} sockpp)