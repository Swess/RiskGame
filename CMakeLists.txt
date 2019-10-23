cmake_minimum_required(VERSION 3.14)
project(Team12_DominationGame)

set(CMAKE_CXX_STANDARD 17)

# Include all subdirectories CMakeList here
# They should append compilation to the SRC_FILES list
include(src/Map/CMakeLists.txt)
include(src/Dice/CMakeLists.txt)
include(src/MapLoader/CMakeLists.txt)
include(src/Cards/CMakeLists.txt)
include(src/Player/CMakeLists.txt)
include(src/Terminal/CMakeLists.txt)

add_executable(Team12_DominationGame src/main.cpp src/exceptions.h ${SRC_FILES})

add_custom_command(
        TARGET Team12_DominationGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/ressources $<TARGET_FILE_DIR:Team12_DominationGame>/ressources
)
