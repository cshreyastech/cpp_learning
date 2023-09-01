# https://www.youtube.com/watch?v=2hNdkYInj4g

cmake_minimum_required(VERSION 3.5)
project(TheCherno)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -std=c++2a -DMINIGLOG=ON")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++2a -DMINIGLOG=ON")
set(asio_header_dir "${PROJECT_SOURCE_DIR}/external/asio-1.18.2/include")


add_executable(SimpleExampleSync ${PROJECT_SOURCE_DIR}/01-SimpleExample/src/SimpleExampleSync.cpp)
target_include_directories(SimpleExampleSync 
  PRIVATE ${asio_header_dir}
)

add_executable(SimpleExampleAsync ${PROJECT_SOURCE_DIR}/01-SimpleExample/src/SimpleExampleAsync.cpp)
target_include_directories(SimpleExampleAsync 
  PRIVATE ${asio_header_dir}
)

set(mmo_net_common_header_dir "${PROJECT_SOURCE_DIR}/02-MMO/NetCommon/include")


add_executable(02-MMO-SimpleServer ${PROJECT_SOURCE_DIR}/02-MMO/NetServer/src/SimpleServer.cpp)
target_include_directories(02-MMO-SimpleServer 
  PRIVATE ${asio_header_dir}
  PRIVATE ${mmo_net_common_header_dir}
)


add_executable(02-MMO-SimpleClient ${PROJECT_SOURCE_DIR}/02-MMO/NetClient/src/SimpleClient.cpp)
target_include_directories(02-MMO-SimpleClient 
  PRIVATE ${asio_header_dir}
  PRIVATE ${mmo_net_common_header_dir}
)