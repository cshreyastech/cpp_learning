# https://www.youtube.com/watch?v=18c3MTX0PK0&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb

cmake_minimum_required(VERSION 3.5)
project(TheCherno)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -std=c++2a")
set(source_dir "${PROJECT_SOURCE_DIR}/src/")


add_executable(04-hello_world ${source_dir}/04-hello_world.cpp)
add_executable(16-pointers ${source_dir}/16-pointers.cpp)
add_executable(40-operator_overloading ${source_dir}/40-operator_overloading.cpp)
add_executable(42-object_lifetime ${source_dir}/42-object_lifetime.cpp)
add_executable(43-smart_pointers ${source_dir}/43-smart_pointers.cpp)
add_executable(52-multiple_return ${source_dir}/52-multiple_return.cpp)
add_executable(53-templates ${source_dir}/53-templates.cpp)
add_executable(55-macros ${source_dir}/55-macros.cpp)
add_executable(58-function_pointers ${source_dir}/58-function_pointers.cpp)

add_executable(62-threads ${source_dir}/62-threads.cpp)
target_link_libraries(62-threads pthread)

add_executable(74-benchmarking ${source_dir}/74-benchmarking.cpp)
add_executable(80-faseter_string ${source_dir}/80-faseter_string.cpp)
add_executable(80-faseter_string_v2 ${source_dir}/80-faseter_string_v2.cpp)
add_executable(81-visual_benchmarking ${source_dir}/81-visual_benchmarking.cpp)
