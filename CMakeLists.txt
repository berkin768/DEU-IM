cmake_minimum_required(VERSION 3.6)
project(Assigment_3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES server.c client.c)
add_executable(Assigment_3 ${SOURCE_FILES} server.c client.c)