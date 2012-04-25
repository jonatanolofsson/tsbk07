cmake_minimum_required(VERSION 2.8)
project(lab4)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -ggdb -Wall -Wextra -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic -ldl")

# add our macro directory to cmake search path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(YAMLCPP_STATIC_LIBRARY ON)

find_package(YamlCpp REQUIRED)
find_package( Eigen3 )
find_package( Boost COMPONENTS thread )


link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${CMAKE_SOURCE_DIR} )
include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${Eigen3_INCLUDE_DIR} )

add_executable(projekt src/main.cpp)
target_link_libraries(projekt
    ${Boost_LIBRARIES}
    CPGL
    ${YAMLCPP_LIBRARY}
)

add_subdirectory(elements)
