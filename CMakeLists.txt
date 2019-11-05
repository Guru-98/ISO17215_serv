cmake_minimum_required (VERSION 2.8.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG  "-g")
add_executable(17215_serv src/server.c src/someip.c src/methods.c src/state.c)
