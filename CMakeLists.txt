cmake_minimum_required(VERSION 3.12)
project(POS_SEM)

find_package(Boost 1.65.0 REQUIRED COMPONENTS system)
find_package(Curses REQUIRED)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CURSES_INCLUDE_DIR})

# Link your application with the ncurses library

add_executable(POS_SEM main.cpp
        Download.cpp
        Download.h
        Enums.h
        Menu.cpp
        Menu.h
        Settings.cpp
        Settings.h
        DownloadManager.cpp
        DownloadManager.h
        DownloadDisplayMenu.cpp
        DownloadDisplayMenu.h)

target_link_libraries(POS_SEM Boost::system)
target_link_libraries(POS_SEM ${CURSES_LIBRARIES})
