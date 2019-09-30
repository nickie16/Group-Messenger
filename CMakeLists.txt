cmake_minimum_required(VERSION 3.15)
project(Group-Messenger)

set(CMAKE_CXX_STANDARD 14)

set (Boost_USE_STATIC_LIBS OFF) #enable dynamic linking
# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost REQUIRED COMPONENTS serialization log log_setup)

include_directories(${Boost_INCLUDE_DIR})

add_executable(Group-Messenger-Client client.cpp TCPClient.cpp)
add_executable(Group-Messenger-Server TCPServer.cpp Group.cpp TCPClient.cpp)
# Note: a target should be already defined using 'add_executable' or 'add_library' 
target_link_libraries(Group-Messenger-Client ${Boost_LIBRARIES})
target_link_libraries(Group-Messenger-Server ${Boost_LIBRARIES})

set (CMAKE_CXX_FLAGS "-DBOOST_LOG_DYN_LINK")
