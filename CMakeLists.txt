cmake_minimum_required(VERSION 3.6)
project(whereami C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -pedantic -Werror -Wextra -Wall -Wextra -O2")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
file(GLOB SOURCES src/*.c)

add_executable(whereami ${SOURCES})
target_link_libraries(whereami curl)
install(TARGETS whereami DESTINATION bin)