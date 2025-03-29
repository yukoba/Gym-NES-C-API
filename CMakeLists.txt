cmake_minimum_required(VERSION 3.10)
project(GymNes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCE_FILES "src/*.cpp" "src/mappers/*.cpp")
add_library(GymNes SHARED ${SOURCE_FILES})
target_include_directories(GymNes PRIVATE include)
