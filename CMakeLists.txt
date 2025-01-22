cmake_minimum_required(VERSION 3.10)
project(LadebalkenPlus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(LadebalkenPlus src/main.cpp)

target_link_libraries(LadebalkenPlus sfml-graphics sfml-window sfml-system)
