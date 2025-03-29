cmake_minimum_required(VERSION 3.14)
project(GymNes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCE_FILES "src/*.cpp" "src/mappers/*.cpp")
add_library(GymNes SHARED ${SOURCE_FILES})
target_compile_definitions(GymNes PRIVATE GymNes_LIBRARY_BUILDING_DLL)
target_include_directories(GymNes PRIVATE include)
