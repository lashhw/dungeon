cmake_minimum_required(VERSION 3.16)
project(Dungeon)
set(CMAKE_CXX_STANDARD 14)
include_directories(includes)
include_directories(header)
add_executable(
        Dungeon
        main.cpp
        header/Chest.h
        header/Dungeon.h
        header/GameCharactor.h
        header/Item.h
        header/Monster.h
        header/NPC.h
        header/Object.h
        header/Player.h
        header/Room.h
        source/Chest.cpp
        source/Dungeon.cpp
        source/GameCharactor.cpp
        source/Item.cpp
        source/Monster.cpp
        source/NPC.cpp
        source/Object.cpp
        source/Player.cpp
        source/Room.cpp
)
