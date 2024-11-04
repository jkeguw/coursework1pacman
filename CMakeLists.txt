cmake_minimum_required(VERSION 3.29)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include_directories(include)


add_executable(untitled src/main.cpp
        src/Position.cpp
        include/Position.h
        include/GameConfig.h
        src/GameConfig.cpp
        include/Character.h
        src/Character.cpp
        include/Pacman.h
        src/Pacman.cpp
        include/Ghost.h
        src/Ghost.cpp
        include/Game.h
        src/Game.cpp
)
