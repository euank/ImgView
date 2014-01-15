cmake_minimum_required(VERSION 2.6)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(imgview)
add_subdirectory(src)

set_target_properties(imgview PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -g -std=c++0x")

add_custom_target(run COMMAND imgview cube.img DEPENDS imgview WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})

add_custom_target(docs COMMAND doxygen doxygen.conf WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
