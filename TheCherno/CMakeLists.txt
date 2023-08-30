# https://www.youtube.com/watch?v=18c3MTX0PK0&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb

cmake_minimum_required(VERSION 3.5)
project(TheCherno)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -std=c++2a")
set(source_dir "${PROJECT_SOURCE_DIR}/src/")


add_executable(04-hello_world ${source_dir}/04-hello_world.cpp)
add_executable(52-multiple_return ${source_dir}/52-multiple_return.cpp)
add_executable(58-function_pointers ${source_dir}/58-function_pointers.cpp)

add_executable(62-threads ${source_dir}/62-threads.cpp)
target_link_libraries(62-threads pthread)