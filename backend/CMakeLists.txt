cmake_minimum_required(VERSION 3.10)
project(SnakeBackend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB SOURCES src/*.cpp)

add_executable(snakebackend ${SOURCES})

target_include_directories(snakebackend PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/json
    ${CMAKE_SOURCE_DIR}/include
)

find_package(Threads REQUIRED)
target_link_libraries(snakebackend PRIVATE Threads::Threads)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Link OpenSSL
target_link_libraries(snakebackend PRIVATE OpenSSL::Crypto)