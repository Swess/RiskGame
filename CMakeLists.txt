cmake_minimum_required(VERSION 3.14)
project(Team12_DominationGame)

set(CMAKE_CXX_STANDARD 17)

# Include all subdirectories CMakeList here
# They should append compilation to the SRC_FILES list
include(src/Map/CMakeLists.txt)
include(src/DiceRoller/CMakeLists.txt)

add_executable(Team12_DominationGame src/main.cpp ${SRC_FILES})