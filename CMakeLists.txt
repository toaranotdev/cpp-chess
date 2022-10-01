# requires at least cmake 3.10
cmake_minimum_required(VERSION 3.10)

# requires c++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# debug shit
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
project(cpp-chess VERSION 2.0)

add_executable(${PROJECT_NAME} ${SRC_FILES})
# compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

# header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# sfml stuff
find_package(SFML REQUIRED COMPONENTS graphics audio system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-system)
