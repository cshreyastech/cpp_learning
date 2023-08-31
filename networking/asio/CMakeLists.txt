# https://www.youtube.com/watch?v=2hNdkYInj4g

cmake_minimum_required(VERSION 3.5)
project(TheCherno)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -std=c++2a -DMINIGLOG=ON")
set(asio_header_dir "${PROJECT_SOURCE_DIR}/external/asio-1.28.1/include")


add_executable(SimpleExampleSync ${PROJECT_SOURCE_DIR}/SimpleExample/src/SimpleExampleSync.cpp)

target_include_directories(SimpleExampleSync 
  PRIVATE ${asio_header_dir}
)

add_executable(SimpleExampleAsync ${PROJECT_SOURCE_DIR}/SimpleExample/src/SimpleExampleAsync.cpp)

target_include_directories(SimpleExampleAsync 
  PRIVATE ${asio_header_dir}
)


# add_executable(52-multiple_return ${source_dir}/52-multiple_return.cpp)
# add_executable(58-function_pointers ${source_dir}/58-function_pointers.cpp)

# add_executable(62-threads ${source_dir}/62-threads.cpp)
# target_link_libraries(62-threads pthread)