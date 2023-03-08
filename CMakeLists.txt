cmake_minimum_required(VERSION 3.24)

include("cmake/setup.cmake")

project(d2bs LANGUAGES CXX VERSION ${D2BS_VERSION})
message(STATUS "Building D2BS v${D2BS_VERSION}")

add_subdirectory(d2bs)
add_subdirectory(jspp)
add_subdirectory(vendor)
