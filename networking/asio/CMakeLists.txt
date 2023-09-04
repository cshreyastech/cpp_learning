# https://www.youtube.com/watch?v=2hNdkYInj4g

cmake_minimum_required(VERSION 3.5)
project(TheCherno)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -std=c++2a -DMINIGLOG=ON")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++2a -DMINIGLOG=ON")
# set(asio_header_dir "${PROJECT_SOURCE_DIR}/external/asio-1.18.2/include")
set(asio_header_dir "${PROJECT_SOURCE_DIR}/external/asio-1.28.1/include")


# add_executable(SimpleExampleSync ${PROJECT_SOURCE_DIR}/01-SimpleExample/src/SimpleExampleSync.cpp)
# target_include_directories(SimpleExampleSync 
#   PRIVATE ${asio_header_dir}
# )

# add_executable(SimpleExampleAsync ${PROJECT_SOURCE_DIR}/01-SimpleExample/src/SimpleExampleAsync.cpp)
# target_include_directories(SimpleExampleAsync 
#   PRIVATE ${asio_header_dir}
# )

# set(mmo_net_common_header_dir "${PROJECT_SOURCE_DIR}/02-MMO/NetCommon/include")

# add_executable(02-NetServer-SimpleServer ${PROJECT_SOURCE_DIR}/02-MMO/NetServer/src/SimpleServer.cpp)
# target_include_directories(02-NetServer-SimpleServer
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${mmo_net_common_header_dir}
# )

# add_executable(02-NetClient-SimpleClient ${PROJECT_SOURCE_DIR}/02-MMO/NetClient/src/SimpleClient.cpp)
# target_include_directories(02-NetClient-SimpleClient 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${mmo_net_common_header_dir}
# )



set(target_common_libraries
  PRIVATE glfw 
  PRIVATE GLU 
  PRIVATE GL 
  PRIVATE Xrandr 
  # PRIVATE Xxf86vm 
  PRIVATE Xi 
  PRIVATE Xinerama 
  PRIVATE X11 
  PRIVATE rt 
  PRIVATE dl
  PRIVATE pthread
  PRIVATE GLEW
  PRIVATE z
  PRIVATE png
  )

set(mmo_server_header_dir "${PROJECT_SOURCE_DIR}/03-olcPGEX/MMO_Server/include")
add_executable(03-olcPGEX_Server  
  ${PROJECT_SOURCE_DIR}/03-olcPGEX/MMO_Server/src/MMO_Server.cpp)
  
target_include_directories(03-olcPGEX_Server 
  PRIVATE ${asio_header_dir}
  PRIVATE ${mmo_server_header_dir}
)

target_link_libraries(03-olcPGEX_Server ${target_common_libraries})

add_executable(03-olcPGEX_Client  
  ${PROJECT_SOURCE_DIR}/03-olcPGEX/MMO_Client/src/MMO_Client.cpp)
  
target_include_directories(03-olcPGEX_Client 
  PRIVATE ${asio_header_dir}
  # PRIVATE ./MMO_Server/include
)

target_link_libraries(03-olcPGEX_Client ${target_common_libraries})