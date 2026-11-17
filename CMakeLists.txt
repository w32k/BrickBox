cmake_minimum_required(VERSION 3.10)

project(BrickBox)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(BrickBox src/oldmain.c src/main.cpp
                        src/net/sstring.c
                        src/net/oldbytebuf.c src/net/Bytebuf.cpp
                        src/net/net.c src/net/client/clienthandler.c
                        src/net/client/oldclientsock.c src/net/client/ClientSock.cpp
                        src/net/packets/clientbound.c src/net/packets/serverbound.c
                        src/extern/miniz.c)

target_include_directories(BrickBox PRIVATE includes)
if(WIN32)
target_link_libraries(BrickBox Ws2_32)
else()
target_link_libraries(BrickBox)
endif()
