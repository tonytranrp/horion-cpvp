cmake_minimum_required(VERSION 3.12)
project(Horion)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wno-dynamic-class-memaccess -Wno-microsoft-goto -Wno-delete-incomplete /EHsc /Ob1)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/MP /Ob1)
endif()

add_compile_definitions(WIN32_LEAN_AND_MEAN)
add_compile_definitions(NOMINMAX)
if(CMAKE_BUILD_TYPE MATCHES Release)

elseif(CMAKE_BUILD_TYPE MATCHES BetaConf)
    add_compile_definitions(_BETA)
endif()

include_directories(.)
include_directories(include)
include_directories(include/glm/)

link_directories(minhook)

file(GLOB_RECURSE HorionFiles
        "*.h"
        "*.cpp")
add_library(Horion SHARED ${HorionFiles})