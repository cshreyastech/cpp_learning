# https://www.youtube.com/watch?v=2hNdkYInj4g

cmake_minimum_required(VERSION 3.5)
project(cpp_learning)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pthread -std=c++17 -DMINIGLOG=ON")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++2a -DMINIGLOG=ON")
add_subdirectory(external/cereal)
add_subdirectory(external/snappy)

set(asio_header_dir "${PROJECT_SOURCE_DIR}/external/asio-1.28.1/include")
# set(external_dir "${PROJECT_SOURCE_DIR}/external")

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
###################################################

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
  PRIVATE snappy
  )

# set(mmo_server_header_dir "${PROJECT_SOURCE_DIR}/03-olcPGEX/MMO_Server/include")
# add_executable(03-olcPGEX_Server  
#   ${PROJECT_SOURCE_DIR}/03-olcPGEX/MMO_Server/src/MMO_Server.cpp)
  
# target_include_directories(03-olcPGEX_Server 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${mmo_server_header_dir}
# )

# target_link_libraries(03-olcPGEX_Server ${target_common_libraries})

# add_executable(03-olcPGEX_Client  
#   ${PROJECT_SOURCE_DIR}/03-olcPGEX/MMO_Client/src/MMO_Client.cpp)
  
# target_include_directories(03-olcPGEX_Client 
#   PRIVATE ${asio_header_dir}
# )

# target_link_libraries(03-olcPGEX_Client ${target_common_libraries})

###################################################
# set(ros_ml_server_header_dir "${PROJECT_SOURCE_DIR}/04-ROS_ML/ROS_ML_Server/include")

# set(ros_ml_server 04-ros_ml_Server)
# add_executable(${ros_ml_server}
#   ${PROJECT_SOURCE_DIR}/04-ROS_ML/ROS_ML_Server/src/ROS_ML_Server.cpp)
  
# target_include_directories(${ros_ml_server} 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${ros_ml_server_header_dir}
# )

# target_link_libraries(${ros_ml_server} ${target_common_libraries})

# set(ros_ml_client 04-ros_ml_Client)
# add_executable(${ros_ml_client}
#   ${PROJECT_SOURCE_DIR}/04-ROS_ML/ROS_ML_Client/src/ROS_ML_Client.cpp)
  
# target_include_directories(${ros_ml_client} 
#   PRIVATE ${asio_header_dir}
# )

# target_link_libraries(${ros_ml_client} ${target_common_libraries})
###################################################
# set(SNAPPY_BUILD_BENCHMARKS OFF CACHE BOOL "")
# set(SNAPPY_BUILD_TESTS OFF CACHE BOOL "")
# add_subdirectory(external/snappy)
# add_subdirectory(external/cereal)

# set(ros_ml_client_header_dir "${PROJECT_SOURCE_DIR}/05-ros_ml/ros_ml_client/include")
# set(ros_ml_server_header_dir "${PROJECT_SOURCE_DIR}/05-ros_ml/ros_ml_server/include")

# set(ros_ml_server 05-ros_ml_server)
# add_executable(${ros_ml_server}
#   ${PROJECT_SOURCE_DIR}/05-ros_ml/ros_ml_server/src/ros_ml_server.cpp)
  
# target_include_directories(${ros_ml_server} 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${ros_ml_server_header_dir}
#   PRIVATE ${external_dir}/snappy
# )

# target_link_libraries(${ros_ml_server} 
# PRIVATE pthread
# PRIVATE snappy
# )


# set(ros_ml_client 05-ros_ml_client)
# add_executable(${ros_ml_client}
#   ${external_dir}/glad/glad.c
#   ${PROJECT_SOURCE_DIR}/05-ros_ml/ros_ml_client/src/game_engine.cpp
#   ${PROJECT_SOURCE_DIR}/05-ros_ml/ros_ml_client/src/ros_ml_client.cpp
#   )
  
# target_include_directories(${ros_ml_client} 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${external_dir}
#   PRIVATE ${external_dir}/snappy
#   PRIVATE ${ros_ml_server_header_dir}
#   PRIVATE ${ros_ml_client_header_dir}
# )

# target_link_libraries(${ros_ml_client} ${target_common_libraries})
#############################################################
# set(ros_ml_client_header_dir "${PROJECT_SOURCE_DIR}/06-ros_ml/ros_ml_client/include")
# set(ros_ml_server_header_dir "${PROJECT_SOURCE_DIR}/06-ros_ml/ros_ml_server/include")

# set(ros_ml_server 06-ros_ml_server)
# add_executable(${ros_ml_server}
#   ${PROJECT_SOURCE_DIR}/06-ros_ml/ros_ml_server/src/ros_ml_server.cpp)
  
# target_include_directories(${ros_ml_server} 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${ros_ml_server_header_dir}
# )

# target_link_libraries(${ros_ml_server} 
#   PRIVATE pthread
# )


# set(ros_ml_client 06-ros_ml_client)
# add_executable(${ros_ml_client}
#   ${PROJECT_SOURCE_DIR}/06-ros_ml/ros_ml_client/src/game_engine.cpp
#   ${PROJECT_SOURCE_DIR}/06-ros_ml/ros_ml_client/src/ros_ml_client.cpp
#   )
  
# target_include_directories(${ros_ml_client} 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${ros_ml_server_header_dir}
#   PRIVATE ${ros_ml_client_header_dir}
# )

# target_link_libraries(${ros_ml_client} 
#   PRIVATE pthread
# )
#############################################################
# Not working for now
# set(ros_ml_client_header_dir "${PROJECT_SOURCE_DIR}/07-ros_ml/ros_ml_client/include")
# set(ros_ml_server_header_dir "${PROJECT_SOURCE_DIR}/07-ros_ml/ros_ml_server/include")

# set(ros_ml_server 07-ros_ml_server)
# add_executable(${ros_ml_server}
#   ${PROJECT_SOURCE_DIR}/07-ros_ml/ros_ml_server/src/ros_ml_server.cpp)
  
# target_include_directories(${ros_ml_server} 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${ros_ml_server_header_dir}
#   PRIVATE ${external_dir}/snappy
# )

# target_link_libraries(${ros_ml_server} 
#   PRIVATE pthread
#   PRIVATE snappy
# )



# set(ros_ml_client 07-ros_ml_client)
# add_executable(${ros_ml_client}
#   ${PROJECT_SOURCE_DIR}/07-ros_ml/ros_ml_client/src/game_engine.cpp
#   ${PROJECT_SOURCE_DIR}/07-ros_ml/ros_ml_client/src/ros_ml_client.cpp
#   )
  
# target_include_directories(${ros_ml_client} 
#   PRIVATE ${asio_header_dir}
#   PRIVATE ${ros_ml_server_header_dir}
#   PRIVATE ${ros_ml_client_header_dir}
#   PRIVATE ${external_dir}/snappy
# )

# target_link_libraries(${ros_ml_client} 
#   PRIVATE pthread
#   PRIVATE snappy
# )
# #############################################################
# set(serialization 08-serialization-json)
# add_executable(${serialization}
#   ${PROJECT_SOURCE_DIR}/08-serialization/src/serialization_json.cpp
#   )
  
# target_include_directories(${serialization} 
#   PRIVATE ${external_dir}
#   PRIVATE ${Snappy_SOURCE_DIR}/snappy
#   PRIVATE ${cereal_SOURCE_DIR}/include
# )

# target_link_libraries(${serialization} 
#   PRIVATE pthread
#   PRIVATE snappy
# )
# #############################################################
# set(serialization 08-serialization-binary)
# add_executable(${serialization}
#   ${PROJECT_SOURCE_DIR}/08-serialization/src/serialization_binary.cpp
#   )
  
# target_include_directories(${serialization} 
#   PRIVATE ${external_dir}
#   PRIVATE ${Snappy_SOURCE_DIR}/snappy
#   PRIVATE ${cereal_SOURCE_DIR}/include
# )

# target_link_libraries(${serialization} 
#   PRIVATE pthread
#   PRIVATE snappy
# )
#############################################################
# set(serialization 08_serialization_binary_vertex)
# add_executable(${serialization}
#   ${PROJECT_SOURCE_DIR}/08-serialization/src/serialization_binary_vertex.cpp
#   )
  
# target_include_directories(${serialization} 
#   PRIVATE ${external_dir}
#   PRIVATE ${Snappy_SOURCE_DIR}/snappy
#   PRIVATE ${cereal_SOURCE_DIR}/include
# )

# target_link_libraries(${serialization} 
#   PRIVATE pthread
#   PRIVATE snappy
# )
# #############################################################
set(serialization 08_serialization_binary_compress)
add_executable(${serialization}
  ${PROJECT_SOURCE_DIR}/08-serialization/src/serialization_binary_compress.cpp
  )
  
target_include_directories(${serialization} 
  PRIVATE ${external_dir}
  PRIVATE ${Snappy_SOURCE_DIR}
  PRIVATE ${cereal_SOURCE_DIR}/include
)

target_link_libraries(${serialization} 
  PRIVATE pthread
  PRIVATE snappy
)
set_target_properties(${serialization} PROPERTIES
  CXX_STANDARD 17  # or the C++ standard you are using
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(${serialization} PRIVATE -O3)
#############################################################
set(serialization 08_serialization_binary_vertex_struct_uniqure_ptr)
add_executable(${serialization}
  ${PROJECT_SOURCE_DIR}/08-serialization/src/serialization_binary_vertex_struct_uniqure_ptr.cpp
  )
  
target_include_directories(${serialization} 
  PRIVATE ${external_dir}
  PRIVATE ${Snappy_SOURCE_DIR}/snappy
  PRIVATE ${cereal_SOURCE_DIR}/include
)

target_link_libraries(${serialization} 
  PRIVATE pthread
  PRIVATE snappy
)
set_target_properties(${serialization} PROPERTIES
  CXX_STANDARD 17  # or the C++ standard you are using
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(${serialization} PRIVATE -O3)
#############################################################
set(serialization 08_serialization_binary_rawcompress)
add_executable(${serialization}
  ${PROJECT_SOURCE_DIR}/08-serialization/src/serialization_binary_rawcompress.cpp
  )
  
target_include_directories(${serialization} 
  PRIVATE ${external_dir}
  PRIVATE ${Snappy_SOURCE_DIR}/snappy
  PRIVATE ${cereal_SOURCE_DIR}/include
)

target_link_libraries(${serialization} 
  PRIVATE pthread
  PRIVATE snappy
)
set_target_properties(${serialization} PROPERTIES
  CXX_STANDARD 17  # or the C++ standard you are using
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

target_compile_options(${serialization} PRIVATE -O3)
#############################################################
set(serialization 08_serialization_binary_optimized)
add_executable(${serialization}
  ${PROJECT_SOURCE_DIR}/08-serialization/src/serialization_binary_optimized.cpp
  )
  
target_include_directories(${serialization} 
  PRIVATE ${external_dir}
  PRIVATE ${Snappy_SOURCE_DIR}/snappy
  PRIVATE ${cereal_SOURCE_DIR}/include
)

target_link_libraries(${serialization} 
  PRIVATE pthread
  PRIVATE snappy
)
set_target_properties(${serialization} PROPERTIES
  CXX_STANDARD 17  # or the C++ standard you are using
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

# target_compile_options(${serialization} PRIVATE -O3)
#############################################################
# Compilation error
# set(serialization 08_serialization_smart_ptr)
# add_executable(${serialization}
#   ${PROJECT_SOURCE_DIR}/08-serialization/src/serialization_smart_ptr.cpp
#   )
  
# target_include_directories(${serialization} 
#   PRIVATE ${external_dir}
#   # PRIVATE ${Snappy_SOURCE_DIR}/snappy
#   PRIVATE ${cereal_SOURCE_DIR}/include
# )

# target_link_libraries(${serialization} 
#   PRIVATE pthread
#   # PRIVATE snappy
# )
#############################################################
set(ros_ml_client_header_dir "${PROJECT_SOURCE_DIR}/09-ros_ml/ros_ml_client/include")
set(ros_ml_server_header_dir "${PROJECT_SOURCE_DIR}/09-ros_ml/ros_ml_server/include")

set(ros_ml_server 09-ros_ml_server)
add_executable(${ros_ml_server}
  ${PROJECT_SOURCE_DIR}/09-ros_ml/ros_ml_server/src/ros_ml_server.cpp)
  
target_include_directories(${ros_ml_server} 
  PRIVATE ${asio_header_dir}
  PRIVATE ${ros_ml_server_header_dir}
  PRIVATE ${external_dir}/snappy
  PRIVATE ${cereal_SOURCE_DIR}/include
)

target_link_libraries(${ros_ml_server} 
  PRIVATE pthread
  PRIVATE snappy
)

set_target_properties(${ros_ml_server} PROPERTIES
  CXX_STANDARD 17  # or the C++ standard you are using
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)
target_compile_options(${ros_ml_server} PRIVATE -O3)

set(ros_ml_client 09-ros_ml_client)
add_executable(${ros_ml_client}
  ${PROJECT_SOURCE_DIR}/09-ros_ml/ros_ml_client/src/game_engine.cpp
  ${PROJECT_SOURCE_DIR}/09-ros_ml/ros_ml_client/src/ros_ml_client.cpp
  )
  
target_include_directories(${ros_ml_client} 
  PRIVATE ${asio_header_dir}
  PRIVATE ${ros_ml_server_header_dir}
  PRIVATE ${ros_ml_client_header_dir}
  PRIVATE ${external_dir}/snappy
  PRIVATE ${cereal_SOURCE_DIR}/include
)

target_link_libraries(${ros_ml_client} 
  PRIVATE pthread
  PRIVATE snappy
)

set_target_properties(${ros_ml_client} PROPERTIES
  CXX_STANDARD 17  # or the C++ standard you are using
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)
target_compile_options(${ros_ml_client} PRIVATE -O3)
#############################################################