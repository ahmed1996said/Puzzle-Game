cmake_minimum_required(VERSION 2.8.8)
project(puzzle)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wno-varargs -Wno-char-subscripts")
include_directories(cimg)
if (APPLE)
    include_directories(/usr/X11/include)
    link_directories(/usr/X11/lib)
endif(APPLE)

# add additional .cpp files to this executable
add_executable(puzzle main.cpp Puzzle.cpp Puzzle.cpp QuadTree.h QuadTree.cpp)
if (APPLE)
    target_link_libraries(puzzle X11)
endif(APPLE)