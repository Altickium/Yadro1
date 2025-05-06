cmake_minimum_required (VERSION 3.8)

project(yadro-test)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra-semi -O1 -g -fsanitize=address -fno-omit-frame-pointer")
endif()

add_executable(main src/main.cpp src/sorter.cpp src/tape.cpp src/config.hpp)
add_executable(tests test/tests.cpp src/sorter.cpp src/tape.cpp src/config.hpp)