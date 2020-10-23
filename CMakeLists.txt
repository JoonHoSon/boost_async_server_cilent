cmake_minimum_required(VERSION 3.10)
project(boost_server)
project(boost_client)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads)

include_directories(boost_server ${Boost_INCLUDE_DIRS})
add_executable(boost_server src/boost_server.cpp)
target_link_libraries(boost_server ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

include_directories(boost_client ${Boost_INCLUDE_DIRS})
add_executable(boost_client src/boost_client.cpp)
target_link_libraries(boost_client ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

